#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }
    std::cout << "Connected to server " << SERVER_IP << ":" << SERVER_PORT << "\n";
    std::cout << "Enter message: ";
    if (fgets(buffer, BUFFER_SIZE, stdin) != nullptr) {
        if (write(sockfd, buffer, strlen(buffer)) < 0) {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }
    int n = read(sockfd, buffer, BUFFER_SIZE - 1);
    if (n < 0) {
        perror("read");
        exit(EXIT_FAILURE);
    }
    buffer[n] = '\0';
    std::cout << "Server reply: " << buffer << "\n";
    close(sockfd);
    return 0;
}
