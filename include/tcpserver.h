#pragma once

#include <arpa/inet.h>
#include <memory.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <vector>

template <typename T> class TCPServer {
  public:
    TCPServer(int nServerPort, int nLengthOfQueueOfListen = 100,
              const char *strBoundIP = NULL);

    virtual ~TCPServer();
    void AddClient(const std::string &clientIP, const std::string &clientPort);
    int InitConnection(std::string clientIP, std::string clientPort);
    int Run();
    virtual void HandleFunction(int client_sock);

  private:
    int server_port_;
    char *str_boundIP_;
    int length_of_queue_of_listen_;
    std::vector<std::pair<std::string, std::string>> clients_;
};