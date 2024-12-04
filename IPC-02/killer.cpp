#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "Fork failed" << std::endl;
        return 1;
    } else if (pid == 0) {
        std::cout << "Child process: PID=" << getpid() 
                  << ", trying to kill parent PID=" << getppid() << std::endl;

        if (kill(getppid(), SIGKILL) == 0) {
            std::cout << "Child process successfully sent SIGKILL to parent." << std::endl;
        } else {
            perror("Child process failed to kill parent");
        }

        std::cout << "Child process is still running (PID=" << getpid() << ")." << std::endl;
        return 0;
    } else {
        std::cout << "Parent process: PID=" << getpid() 
                  << ", waiting for child." << std::endl;

        wait(NULL);
        std::cout << "Parent process: child has terminated." << std::endl;
    }

    return 0;
}
