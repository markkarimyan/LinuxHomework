#include <iostream>
#include <string>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUF_SIZE 1024
#define NAME_MAX 32

void receiveMessages(int sock) {
    char buffer[BUF_SIZE];
    while (true) {
        int bytes = recv(sock, buffer, BUF_SIZE, 0);
        if (bytes <= 0) break;
        buffer[bytes] = '\0';
        std::cout << buffer;
    }
}

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return 1;
    
    sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(PORT);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    if (connect(sock, (sockaddr*)&servAddr, sizeof(servAddr)) < 0) return 1;
    
    std::string userName;
    std::cout << "Enter your name: ";
    std::getline(std::cin, userName);
    if (userName.size() >= NAME_MAX)
        userName = userName.substr(0, NAME_MAX - 1);
    send(sock, userName.c_str(), userName.size(), 0);
    
    std::thread recvThread(receiveMessages, sock);
    
    std::string input;
    while (std::getline(std::cin, input)) {
        send(sock, input.c_str(), input.size(), 0);
        if (input == "/exit") break;
    }
    
    close(sock);
    recvThread.join();
    return 0;
}
