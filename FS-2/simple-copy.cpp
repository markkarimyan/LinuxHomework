#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source-file name> <destination-file name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    
    int source_fd = open(argv[1], O_RDONLY);
    int dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (source_fd == -1 || dest_fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    
    char buffer[1024];
    ssize_t bytes_read;

    
    while ((bytes_read = read(source_fd, buffer, sizeof(buffer))) > 0) {
        if (write(dest_fd, buffer, bytes_read) != bytes_read) {
            perror("Error writing to file");
            exit(EXIT_FAILURE);
        }
    }

    
    if (bytes_read == -1) {
        perror("Error reading file");
    }

    
    close(source_fd);
    close(dest_fd);

    return 0;
}
