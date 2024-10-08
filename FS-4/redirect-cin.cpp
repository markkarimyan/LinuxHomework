#include <iostream>
#include <fstream>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>

void initialize(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input-file>\n";
        exit(1);
    }

    int fileDescriptor = open(argv[1], O_RDONLY);
    if (fileDescriptor < 0) {
        perror("Error opening file");
        exit(errno);
    }

    if (dup2(fileDescriptor, STDIN_FILENO) < 0) {
        perror("Error redirecting stdin");
        close(fileDescriptor);
        exit(errno);
    }

    
    close(fileDescriptor);
}

int main(int argc, char** argv)
{
    
    initialize(argc, argv);

    std::string input;
    std::cin >> input;

    std::string reversed(input.rbegin(), input.rend());

    std::cout << reversed << std::endl;

    return 0;
}
