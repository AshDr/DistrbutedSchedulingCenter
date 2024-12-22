#include "tcpserver.h"
#include <cstring>
#include <iostream>
#include <thread>

TCPServer::TCPServer(int nServerPort, int nLengthOfQueueOfListen,
                     const char *strBoundIP) {
  server_port_ = nServerPort;
  length_of_queue_of_listen_ = nLengthOfQueueOfListen;

  if (NULL == strBoundIP) {
    str_boundIP_ = NULL;
  } else {
    int length = strlen(strBoundIP);
    str_boundIP_ = new char[length + 1];
    memcpy(str_boundIP_, strBoundIP, length + 1);
  }
}

TCPServer::~TCPServer() {
  if (str_boundIP_ != NULL) {
    delete[] str_boundIP_;
  }
}

int TCPServer::Run() {
  int nListenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
  if (-1 == nListenSocket) {
    std::cout << "socket error" << std::endl;
    return -1;
  }

  sockaddr_in ServerAddress;
  memset(&ServerAddress, 0, sizeof(sockaddr_in));
  ServerAddress.sin_family = AF_INET;

  if (NULL == str_boundIP_) {
    ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
  } else {
    if (::inet_pton(AF_INET, str_boundIP_, &ServerAddress.sin_addr) != 1) {
      std::cout << "inet_pton error" << std::endl;
      ::close(nListenSocket);
      return -1;
    }
  }

  ServerAddress.sin_port = htons(server_port_);

  if (::bind(nListenSocket, (sockaddr *)&ServerAddress, sizeof(sockaddr_in)) ==
      -1) {
    std::cout << "bind error" << std::endl;
    ::close(nListenSocket);
    return -1;
  }

  if (::listen(nListenSocket, length_of_queue_of_listen_) == -1) {
    std::cout << "listen error" << std::endl;
    ::close(nListenSocket);
    return -1;
  }
  std::cout << "Server Listening on port: " << server_port_ << std::endl;
  while (1) {
    struct sockaddr_in ClientAddress;
    socklen_t nSize = sizeof(ClientAddress);
    int client_sock =
        ::accept(nListenSocket, (sockaddr *)&ClientAddress, &nSize); //
    if (client_sock < 0) {
      std::cout << "accept error" << std::endl;
      continue;
    }
    std::cout << "Client socket " << client_sock << " connected!" << std::endl;
    std::thread client_handle_thread(
        [this, client_sock]() { HandleFunction(client_sock); });
    client_handle_thread.detach();
  }
  ::close(nListenSocket);
  return 0;
}
void TCPServer::HandleFunction(int client_sock) {
  std::cout << "TCPServer<T>::ServerFunction: You should implement this "
               "function in your derived class"
            << std::endl;
  close(client_sock);
}