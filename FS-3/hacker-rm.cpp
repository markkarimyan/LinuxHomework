#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstdio>

int main(int argc, char* argv[]) {
    
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <provide the name of file to delete\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    
    struct stat file_info;
    if (stat(argv[1], &file_info) == -1) {
        perror("Error getting file info");
        exit(EXIT_FAILURE);
    }

    
    int fileToDel = open(argv[1], O_WRONLY);
    if (fileToDel == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    
    char null_byte = '\0';
    for (off_t i = 0; i < file_info.st_size; ++i) {
        if (write(fileToDel, &null_byte, 1) != 1) {
            perror("Error writing null byte to file");
            close(fileToDel);
            exit(EXIT_FAILURE);
        }
    }

    
    close(fileToDel);

    
    if (unlink(argv[1]) == -1) {
        perror("Error deleting file");
        exit(EXIT_FAILURE);
    }

    printf("File '%s' erased and deleted successfully.\n", argv[1]);
    return 0;
}
