#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(8888);
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Bind error");
        exit(EXIT_FAILURE);
    }
    if (listen(server_socket, 10) < 0) {
        perror("Listen error");
        exit(EXIT_FAILURE);
    }
    std::cout << "Server running. Waiting for connections...\n";
    while (true) {
        int client_socket;
        struct sockaddr_in client_address;
        unsigned int client_addr_len = sizeof(client_address);
        client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_addr_len);
        if (client_socket < 0) {
            perror("Accept error");
            continue;
        }
        std::cout << "Connected client from: " << inet_ntoa(client_address.sin_addr) << "\n";
        char buffer[1024];
        int received_bytes = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (received_bytes < 0) {
            perror("Receive error");
            close(client_socket);
            continue;
        }
        buffer[received_bytes] = '\0';
        std::string request(buffer);
        std::cout << "Request received: " << request << "\n";
        std::istringstream iss(request);
        std::string operation;
        double operand1, operand2;
        iss >> operation >> operand1 >> operand2;
        double result = 0;
        bool valid = true;
        std::string response;
        if (operation == "ADD")
            result = operand1 + operand2;
        else if (operation == "SUB")
            result = operand1 - operand2;
        else if (operation == "MUL")
            result = operand1 * operand2;
        else if (operation == "DIV") {
            if (operand2 == 0) {
                valid = false;
                response = "Error: Division by zero.\n";
            } else
                result = operand1 / operand2;
        } else {
            valid = false;
            response = "Error: Unknown command.\n";
        }
        if (valid)
            response = std::to_string(result) + "\n";
        int sent_bytes = send(client_socket, response.c_str(), response.size(), 0);
        if (sent_bytes < 0)
            perror("Send error");
        close(client_socket);
    }
    close(server_socket);
    return 0;
}
