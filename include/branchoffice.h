#pragma once

#include "task.h"
#include "tcpclient.h"
#include "vehicle.h"
#include <queue>
#include <vector>
const int MAX_BUFFER_SIZE = 1024;
class BranchOffice : public TCPClient{
  public:
    BranchOffice(int server_port, char * server_ip, int vehicle_cnt);
    void DispatchTask(Task task, int client_sock);
    void TaskWork(Task task, int client_sock);
    void HandleFunction(int client_sock) override;
  private:
    int branch_id_;
    int total_vehicles;
    std::queue<Vehicle> free_vehicles_;
};