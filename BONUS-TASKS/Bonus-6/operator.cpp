#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>

int main() {
    int pipefd[2];
    pid_t pid1, pid2;

    pipe(pipefd);

    pid1 = fork();
    if (pid1 == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        std::cout << "Hello, world!" << std::endl;
        _exit(0);
    }

    pid2 = fork();
    if (pid2 == 0) {
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);

        char buffer[128];
        std::cin.getline(buffer, sizeof(buffer));
        std::cout << "Received: " << buffer << std::endl;
        _exit(0);
    }

    close(pipefd[0]);
    close(pipefd[1]);

    wait(NULL);
    wait(NULL);

    return 0;
}
