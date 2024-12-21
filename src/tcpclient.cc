#include "tcpclient.h"
#include <iostream>

template <typename T>
TCPClient<T>::TCPClient(int nServerPort, const char *strServerIP) {
    server_port_ = nServerPort;
    int nlength = strlen(strServerIP);
    server_ip_ = new char[nlength + 1];
    strcpy(server_ip_, strServerIP);
}

template <typename T> TCPClient<T>::~TCPClient() {
    delete[] server_ip_;
}

template <typename T> int TCPClient<T>::Run() {
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
    T *pT = static_cast<T *>(this);
    while (true) {
        pT->ClientFunction(client_sock);
    }
    ::close(client_sock);
    return 0;
}

template <typename T> void TCPClient<T>::ClientFunction(int nConnectedSocket) {
    std::cout << "TCPClient<T>::ClientFunction: You should implement this "
                 "function in your derived class"
              << std::endl;
}