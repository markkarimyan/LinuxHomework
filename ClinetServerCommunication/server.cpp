#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

pthread_mutex_t cout_mutex = PTHREAD_MUTEX_INITIALIZER;

void* handle_client(void* arg) {
  int client_socket = *(int*)arg;
  delete (int*)arg;
  char buffer[1001];
  while (true) {
    memset(buffer, 0, sizeof(buffer));
    int bytes_received = recv(client_socket, buffer, 1000, 0);
    if (bytes_received <= 0)
      break;
    buffer[bytes_received] = '\0';
    pthread_mutex_lock(&cout_mutex);
    std::cout << "Сообщение от клиента: " << buffer << "\n";
    pthread_mutex_unlock(&cout_mutex);
  }
  close(client_socket);
  return nullptr;
}

int main() {
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
    perror("socket creation error");
    exit(EXIT_FAILURE);
  }
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(8888);
  if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  if (listen(server_socket, 10) < 0) {
    perror("listen failed");
    exit(EXIT_FAILURE);
  }
  std::cout << "Ожидание соединений...\n";
  while (true) {
    struct sockaddr_in client_address;
    socklen_t client_addr_len = sizeof(client_address);
    int* client_socket = new int;
    *client_socket = accept(server_socket, (struct sockaddr*) &client_address, &client_addr_len);
    if (*client_socket < 0) {
      perror("accept failed");
      delete client_socket;
      continue;
    }
    pthread_mutex_lock(&cout_mutex);
    std::cout << "Подключен клиент с адресом: " << inet_ntoa(client_address.sin_addr) << "\n";
    pthread_mutex_unlock(&cout_mutex);
    pthread_t thread_id;
    if (pthread_create(&thread_id, nullptr, handle_client, client_socket) != 0) {
      perror("pthread_create failed");
      close(*client_socket);
      delete client_socket;
      continue;
    }
    pthread_detach(thread_id);
  }
  close(server_socket);
  return 0;
}
