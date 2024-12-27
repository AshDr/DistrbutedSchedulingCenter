#pragma once
#include "task.h"
#include "tcpserver.h"
#include <arpa/inet.h>
#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <netinet/in.h>
#include <queue>
#include <unistd.h>
#include <unordered_map>
#include <vector>

const int MAX_BUFFER_SIZE = 1024;
enum ClientStatus { IDLE, BUSY };

class SchedulingCenter : public TCPServer {
public:
  SchedulingCenter(int server_port, int length_of_queue_of_listen=100,
                   char *bound_ip=NULL);
  ~SchedulingCenter();
  void AssignTask(int client_sock);
  void GenerateReport();
  void HandleFunction(int client_sock) override;
  void HandleStatus(int client_sock, std::vector<uint8_t> &buffer);
  void HandleMonitorTask(int client_sock, std::vector<uint8_t> &buffer);
  void AddTask(int id);
  void UpdatePlugin(std::string plugin_name, std::string plugin_dir);
  template <typename T>
  void SendMessage(int client_sock, std::string message, T msg_type);
private:
  std::unordered_map<int, std::atomic<ClientStatus>> client_status_map;
  std::queue<Task> tasks_;
  std::mutex task_mutex_;
  std::condition_variable task_cv_;
  std::mutex report_mutex_;
  std::vector<std::string> reports_;
  std::mutex free_clients_mutex_;
  std::queue<int> free_clients_;
};