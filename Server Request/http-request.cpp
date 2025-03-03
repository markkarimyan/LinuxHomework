#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#define BUFSIZE 4096

int main() {
    const char* hostname = "httpforever.com";
    const char* port = "80";

    struct addrinfo hints, *res, *p;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;      // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP

    int status = getaddrinfo(hostname, port, &hints, &res);
    if (status != 0) {
        std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
        return 1;
    }

    int sockfd;
    for (p = res; p != nullptr; p = p->ai_next) {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd == -1)
            continue;
        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            continue;
        }
        break;
    }

    if (p == nullptr) {
        std::cerr << "Failed to connect to " << hostname << std::endl;
        freeaddrinfo(res);
        return 2;
    }
    freeaddrinfo(res);

    std::string request = "GET / HTTP/1.1\r\nHost: httpforever.com\r\nConnection: close\r\n\r\n";
    if (send(sockfd, request.c_str(), request.size(), 0) == -1) {
        std::cerr << "Error sending request." << std::endl;
        close(sockfd);
        return 3;
    }

    std::ofstream outFile("httpforever.html", std::ios::binary);
    if (!outFile) {
        std::cerr << "Error opening file for writing." << std::endl;
        close(sockfd);
        return 4;
    }

    char buffer[BUFSIZE];
    ssize_t bytesRead;
    while ((bytesRead = recv(sockfd, buffer, BUFSIZE, 0)) > 0) {
        outFile.write(buffer, bytesRead);
    }
    if (bytesRead < 0)
        std::cerr << "Error receiving data." << std::endl;

    outFile.close();
    close(sockfd);
    std::cout << "Response saved in httpforever.html" << std::endl;
    return 0;
}
