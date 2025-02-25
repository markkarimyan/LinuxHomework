#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <cstdlib>

int main() {
  struct sockaddr_in server_address;
  int client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (client_socket == -1) {
    perror("socket creation error");
    exit(EXIT_FAILURE);
  }
  server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(8888);
  if (connect(client_socket, (struct sockaddr*) &server_address, sizeof(server_address)) == -1) {
    perror("connection failed");
    exit(EXIT_FAILURE);
  }
  std::string mess;
  while (true) {
    std::cout << "Введите сообщение (или 'exit' для выхода): ";
    std::getline(std::cin, mess);
    if (mess == "exit")
      break;
    int sent = send(client_socket, mess.c_str(), mess.size(), 0);
    if (sent == -1) {
      perror("send error");
      break;
    }
  }
  close(client_socket);
  return 0;
}
