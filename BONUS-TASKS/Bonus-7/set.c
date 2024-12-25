#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define SIZE 10000
#define SHM_NAME "/shm_example"

int main() {
    int fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        exit(1);
    }

    int *arr = mmap(NULL, SIZE * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (arr == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    for (int i = 2; i < SIZE; ++i) {
        arr[i] = 1;
    }

    for (int i = 2; i * i < SIZE; ++i) {
        if (arr[i] == 1) {
            for (int j = i * i; j < SIZE; j += i) {
                arr[j] = 0;
            }
        }
    }

    munmap(arr, SIZE * sizeof(int));
    close(fd);

    return 0;
}
