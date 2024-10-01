#include <fcntl.h>   
#include <unistd.h>  
#include <stdio.h>   
#include <stdlib.h>  

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file-path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("Error opening the file");
        exit(EXIT_FAILURE);
    }

    
    char buffer[1024];

    ssize_t bytes_read;
    
    while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0';
        printf("%s", buffer);
    }

    if (bytes_read == -1) {
        perror("Error reading the file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    
    close(fd);
    return 0;
}
