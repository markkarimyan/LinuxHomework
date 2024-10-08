#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <file-path>\n";
        return 1;
    }

    const char* filePath = argv[1];
    const char* firstLine = "first line\n";
    const char* secondLine = "second line\n";

    int fd = open(filePath, O_WRONLY | O_CREAT, 0644);
    if (fd < 0) {
        perror("Error opening file");
        return errno;
    }

    int fd2 = dup(fd);
    if (fd2 < 0) {
        perror("Error duplicating file descriptor");
        close(fd);
        return errno;
    }

    if (write(fd, firstLine, strlen(firstLine)) < 0) {
        perror("Error writing first line");
        close(fd);
        close(fd2);
        return errno;
    }

    if (write(fd2, secondLine, strlen(secondLine)) < 0) {
        perror("Error writing second line");
        close(fd);
        close(fd2);
        return errno;
    }

    close(fd);
    close(fd2);

    return 0;
}
