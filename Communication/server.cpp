#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUF_SIZE 1024
#define NAME_MAX 32

struct Client {
    int sock;
    int id;
    std::string name;
};

std::vector<Client*> clients;
std::mutex clientsMutex;
int currentId = 1;

void broadcast(const std::string &msg, int senderId) {
    std::lock_guard<std::mutex> lock(clientsMutex);
    for (auto c : clients) {
        if (c->id != senderId) {
            send(c->sock, msg.c_str(), msg.size(), 0);
        }
    }
}

void sendUserList(int sock) {
    std::string list = "Online users:\n";
    std::lock_guard<std::mutex> lock(clientsMutex);
    for (auto c : clients) {
        list += c->name + "\n";
    }
    send(sock, list.c_str(), list.size(), 0);
}

void removeClient(int id) {
    std::lock_guard<std::mutex> lock(clientsMutex);
    for (auto it = clients.begin(); it != clients.end(); ++it) {
        if ((*it)->id == id) {
            clients.erase(it);
            break;
        }
    }
}

void handleClient(Client *client) {
    char buffer[BUF_SIZE];
    int bytes = recv(client->sock, buffer, NAME_MAX, 0);
    if (bytes <= 0) {
        close(client->sock);
        delete client;
        return;
    }
    buffer[bytes] = '\0';
    client->name = std::string(buffer);
    std::string joinMsg = client->name + " joined the chat\n";
    broadcast(joinMsg, client->id);
    
    while (true) {
        bytes = recv(client->sock, buffer, BUF_SIZE, 0);
        if (bytes <= 0) break;
        buffer[bytes] = '\0';
        std::string msg(buffer);
        if (msg == "/exit") {
            std::string exitMsg = client->name + " left the chat\n";
            broadcast(exitMsg, client->id);
            break;
        } else if (msg == "/list") {
            sendUserList(client->sock);
        } else {
            std::string fullMsg = client->name + ": " + msg + "\n";
            broadcast(fullMsg, client->id);
        }
    }
    close(client->sock);
    removeClient(client->id);
    delete client;
}

int main() {
    int serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock < 0) return 1;
    
    sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = INADDR_ANY;
    servAddr.sin_port = htons(PORT);
    
    if (bind(serverSock, (sockaddr*)&servAddr, sizeof(servAddr)) < 0) return 1;
    if (listen(serverSock, 10) < 0) return 1;
    
    std::cout << "Server is running on port " << PORT << "\n";
    
    while (true) {
        sockaddr_in cliAddr;
        socklen_t addrLen = sizeof(cliAddr);
        int cliSock = accept(serverSock, (sockaddr*)&cliAddr, &addrLen);
        if (cliSock < 0) continue;
        Client *newClient = new Client;
        newClient->sock = cliSock;
        newClient->id = currentId++;
        {
            std::lock_guard<std::mutex> lock(clientsMutex);
            clients.push_back(newClient);
        }
        std::thread(handleClient, newClient).detach();
    }
    
    close(serverSock);
    return 0;
}
