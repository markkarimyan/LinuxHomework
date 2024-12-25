#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define SIZE 10000
#define SHM_NAME "/shm_example"

int main() {
    int fd = shm_open(SHM_NAME, O_RDONLY, 0666);
    if (fd == -1) {
        perror("shm_open");
        exit(1);
    }

    shm_unlink(SHM_NAME);

    close(fd);
    return 0;
}
