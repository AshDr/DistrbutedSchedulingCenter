#pragma once

#include "task.h"
#include "tcpclient.h"
#include "vehicle.h"
#include <mutex>
#include <queue>
#include <string>
#include <vector>
#include "plugin.h"
const int MAX_BUFFER_SIZE = 1024;
class BranchOffice : public TCPClient {
public:
  BranchOffice(int server_port, char *server_ip, int vehicle_cnt,
               int branch_id);
  void DispatchTask(Task task, int client_sock);
  void TaskWork(Task task, Vehicle vehicle, int client_sock);
  void HandleFunction(int client_sock) override;
  template <typename T>
  void SendMessage(int client_sock, std::string message, T msg_type);
  void SetPlugin(Plugin plugin);
private:
  int branch_id_;
  int total_vehicles;
  std::mutex free_vehicles_mutex_;
  std::queue<Vehicle> free_vehicles_;
  Plugin branch_plugin_;
};