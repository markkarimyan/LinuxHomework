#include <iostream>
#include <unistd.h>
#include <cstring>
#include <chrono>
#include <sys/wait.h>

void do_command(char** argv) {
    pid_t pid = fork();

    if (pid == -1) {
        std::cerr << "Failed to fork\n";
    } else if (pid == 0) {
        execvp(argv[0], argv);

        std::cerr << "Execution failed: " << strerror(errno) << "\n";
        exit(1);

    } else {
        auto start = std::chrono::high_resolution_clock::now();
        int status;
        if (wait(&status) == -1) {
            std::cerr << "Error waiting for child process" << strerror(errno) << "\n";
        } else {
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> time = end - start;

            std::cout << "Command completed with status code " << status << " and took " << time.count() << " seconds\n";
        }
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: ./do-command the_command its_arguments.\n";
        return 1;
    }

    char* cmd_argv[argc];

    for (int i = 1; i < argc; ++i) {
        cmd_argv[i - 1] = argv[i];
    }

    cmd_argv[argc - 1] = nullptr;

    do_command(cmd_argv);

    return 0;
}