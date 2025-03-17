#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <poll.h>
#include <pthread.h>

#define PORT 12345
#define MAX_CLIENTS 100
#define BUFFER_SIZE 1024

void* client_handler(void* arg) {
    int client_fd = *(int*)arg;
    free(arg);
    char buffer[BUFFER_SIZE];
    int n;
    while ((n = read(client_fd, buffer, BUFFER_SIZE - 1)) > 0) {
        buffer[n] = '\0';
        std::cout << "Received from client " << client_fd << ": " << buffer;
        if (write(client_fd, buffer, n) < 0) {
            perror("write");
            break;
        }
    }
    if (n < 0) {
        perror("read");
    }
    std::cout << "Client " << client_fd << " disconnected\n";
    close(client_fd);
    return nullptr;
}

int main() {
    int server_fd, new_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    struct pollfd poll_fds[MAX_CLIENTS];
    int nfds = 1;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    std::cout << "Server listening on port " << PORT << "...\n";
    poll_fds[0].fd = server_fd;
    poll_fds[0].events = POLLIN;
    while (1) {
        int poll_count = poll(poll_fds, nfds, -1);
        if (poll_count < 0) {
            perror("poll");
            break;
        }
        if (poll_fds[0].revents & POLLIN) {
            new_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
            if (new_fd < 0) {
                perror("accept");
                continue;
            }
            std::cout << "Accepted new client: " << new_fd << "\n";
            int *pclient = (int*)malloc(sizeof(int));
            if (!pclient) {
                perror("malloc");
                close(new_fd);
                continue;
            }
            *pclient = new_fd;
            pthread_t tid;
            if (pthread_create(&tid, nullptr, client_handler, pclient) != 0) {
                perror("pthread_create");
                free(pclient);
                close(new_fd);
                continue;
            }
            pthread_detach(tid);
        }
    }
    close(server_fd);
    return 0;
}
