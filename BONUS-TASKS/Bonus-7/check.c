#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define SIZE 10000
#define SHM_NAME "/shm_example"

int main() {
    int n;
    printf("Enter number: ");
    scanf("%d", &n);

    if (n < 2 || n >= SIZE) {
        printf("Number out of bounds\n");
        return 0;
    }

    int fd = shm_open(SHM_NAME, O_RDONLY, 0666);
    if (fd == -1) {
        perror("shm_open");
        exit(1);
    }

    int *arr = mmap(NULL, SIZE * sizeof(int), PROT_READ, MAP_SHARED, fd, 0);
    if (arr == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    if (arr[n] == 1) {
        printf("Yes\n");
    } else {
        printf("No\n");
    }

    munmap(arr, SIZE * sizeof(int));
    close(fd);

    return 0;
}
