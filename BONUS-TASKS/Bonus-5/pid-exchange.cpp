#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {
    int pipefd1[2], pipefd2[2];
    pid_t pid1, pid2;

    pipe(pipefd1);
    pipe(pipefd2);

    pid1 = fork();
    if (pid1 == 0) {
        pid_t pid = getpid();
        write(pipefd1[1], &pid, sizeof(pid));
        close(pipefd1[1]);
        read(pipefd2[0], &pid, sizeof(pid));
        std::cout << "Child 1 received pid from Child 2: " << pid << std::endl;
        close(pipefd2[0]);
        return 0;
    }

    pid2 = fork();
    if (pid2 == 0) {
        pid_t pid = getpid();
        write(pipefd2[1], &pid, sizeof(pid));
        close(pipefd2[1]);
        read(pipefd1[0], &pid, sizeof(pid));
        std::cout << "Child 2 received pid from Child 1: " << pid << std::endl;
        close(pipefd1[0]);
        return 0;
    }

    close(pipefd1[1]);
    close(pipefd2[1]);

    wait(NULL);
    wait(NULL);

    return 0;
}
