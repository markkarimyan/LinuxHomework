#include <iostream>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
//#include <algorithm>

#define BUFFER_SIZE 4096

int main(int argc, char** argv) {
    std::string sourceFile, destinationFile;
    off_t sourceOffset, destOffset;
    size_t numBytes;

    std::cout << "Enter the name of the source file: ";
    std::cin >> sourceFile;

    std::cout << "Enter the name of the destination file: ";
    std::cin >> destinationFile;

    std::cout << "Enter the initial offset in the source file (in bytes): ";
    std::cin >> sourceOffset;

    std::cout << "Enter the initial offset in the destination file (in bytes): ";
    std::cin >> destOffset;

    std::cout << "Enter the number of bytes to copy: ";
    std::cin >> numBytes;

    int srcFd = open(sourceFile.c_str(), O_RDONLY);
    if (srcFd < 0) {
        std::cerr << "Error opening source file!" << std::endl;
        return EXIT_FAILURE;
    }


    int destFd = open(destinationFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (destFd < 0) {
        std::cerr << "Error opening destination file!" << std::endl;
        close(srcFd);
        return EXIT_FAILURE;
    }


    if (lseek(srcFd, sourceOffset, SEEK_SET) < 0) {
        std::cerr << "Error seeking in source file" << std::endl;
        close(srcFd);
        close(destFd);
        return 1;
    }

    if (lseek(destFd, destOffset, SEEK_SET) < 0) {
        std::cerr << "Error seeking in destination file" << std::endl;
        close(srcFd);
        close(destFd);
        return 1;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytesRead, bytesToWrite = numBytes;
    while (bytesToWrite > 0 && (bytesRead = read(srcFd, buffer, std::min(static_cast<ssize_t>(BUFFER_SIZE), bytesToWrite))) > 0) {
        ssize_t bytesWritten = write(destFd, buffer, bytesRead);
        if (bytesWritten < 0) {
            std::cerr << "Error writing to destination file" << std::endl;
            close(srcFd);
            close(destFd);
            return 1;
        }
        bytesToWrite -= bytesWritten;
    }

    if (bytesRead < 0) {
        std::cerr << "Error reading from source file" << std::endl;
    }


    close(srcFd);
    close(destFd);

    std::cout << "Successfully copied " << numBytes << " bytes from the source file to the destination file." << std::endl;

    return 0;

}