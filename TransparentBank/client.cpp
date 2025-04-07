#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

void startClient(const std::string& server_ip, int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return;

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, server_ip.c_str(), &server_addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        close(sock);
        return;
    }

    std::string request;
    while (true) {
        std::cout << "Enter command: ";
        std::getline(std::cin, request);
        if (request == "exit") break;
        send(sock, request.c_str(), request.length(), 0);
        char response[1024] = {0};
        recv(sock, response, 1024, 0);
        std::cout << "Server response: " << response << "\n";
    }
    close(sock);
}

int main() {
    startClient("127.0.0.1", 8080);
    return 0;
}
