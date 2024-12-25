#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <cstring>
#include <errno.h>

void printFileContents(const char* filename) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        std::cerr << "Failed to open file: " << filename << ", error: " << strerror(errno) << std::endl;
        return;
    }

    char buffer[1024];
    ssize_t bytesRead;
    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
        write(STDOUT_FILENO, buffer, bytesRead);
    }

    if (bytesRead == -1) {
        std::cerr << "Error reading file: " << filename << ", error: " << strerror(errno) << std::endl;
    }

    close(fd);
}

void traverseDirectory(const char* dirPath) {
    DIR* dir = opendir(dirPath);
    if (!dir) {
        std::cerr << "Failed to open directory: " << dirPath << ", error: " << strerror(errno) << std::endl;
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        std::string fullPath = std::string(dirPath) + "/" + entry->d_name;

        if (entry->d_type == DT_DIR) {
            traverseDirectory(fullPath.c_str());
        } else if (entry->d_type == DT_REG) {
            std::cout << "File content: " << fullPath << std::endl;
            printFileContents(fullPath.c_str());
        }
    }

    closedir(dir);
}

int main() {
    char dirPath[256];
    std::cout << "Enter directory path: ";
    std::cin.getline(dirPath, sizeof(dirPath));

    traverseDirectory(dirPath);

    return 0;
}
