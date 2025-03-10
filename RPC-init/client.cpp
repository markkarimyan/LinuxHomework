#include <iostream>
#include <string>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>

int main() {
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation error");
        exit(errno);
    }
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(8888);
    if (connect(client_socket, (struct sockaddr*) &server_address, sizeof(server_address)) == -1) {
        perror("Connection error");
        exit(errno);
    }
    std::cout << "Enter command (e.g., ADD 5 3): ";
    std::string message;
    std::getline(std::cin, message);
    int sent_bytes = send(client_socket, message.c_str(), message.size(), 0);
    if (sent_bytes == -1) {
        perror("Send error");
        exit(errno);
    }
    char buffer[1024];
    int received_bytes = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (received_bytes < 0) {
        perror("Receive error");
        exit(errno);
    }
    buffer[received_bytes] = '\0';
    std::cout << "Server response: " << buffer << "\n";
    close(client_socket);
    return 0;
}
