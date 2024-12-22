#include "tcpclient.h"
#include <iostream>

TCPClient::TCPClient(int nServerPort, const char *strServerIP) {
  server_port_ = nServerPort;
  int nlength = strlen(strServerIP);
  server_ip_ = new char[nlength + 1];
  strcpy(server_ip_, strServerIP);
}

TCPClient::~TCPClient() { delete[] server_ip_; }

int TCPClient::Run() {
  int client_sock = ::socket(AF_INET, SOCK_STREAM, 0);
  if (-1 == client_sock) {
    std::cout << "socket error" << std::endl;
    return -1;
  }
  struct sockaddr_in ServerAddress;
  memset(&ServerAddress, 0, sizeof(sockaddr_in));
  ServerAddress.sin_family = AF_INET;
  if (::inet_pton(AF_INET, server_ip_, &ServerAddress.sin_addr) != 1) {
    std::cout << "inet_pton error" << std::endl;
    ::close(client_sock);
    return -1;
  }
  ServerAddress.sin_port = htons(server_port_);
  if (::connect(client_sock, (sockaddr *)&ServerAddress,
                sizeof(ServerAddress)) == -1) {
    std::cout << "client connect error" << std::endl;
    ::close(client_sock);
    return -1;
  }
  HandleFunction(client_sock);
  ::close(client_sock);
  return 0;
}

void TCPClient::HandleFunction(int client_sock) {
  std::cout << "TCPClient<T>::ClientFunction: You should implement this "
               "function in your derived class"
            << std::endl;
}