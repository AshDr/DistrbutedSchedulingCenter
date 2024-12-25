#include "branchoffice.h"
#include "utils.h"
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <mutex>
#include <thread>

BranchOffice::BranchOffice(int server_port, char *server_ip, int vehicle_cnt,
                           int branch_id)
    : TCPClient(server_port, server_ip), total_vehicles(vehicle_cnt),
      branch_id_(branch_id) {
  // std::lock_guard<std::mutex> lock()
  for (int i = 1; i <= vehicle_cnt; i++) {
    free_vehicles_.emplace(i, 0);
  }
}

template <typename T>
void BranchOffice::SendMessage(int client_sock, std::string message,
                               T msg_type) {
  std::vector<uint8_t> buf;
  buf.insert(buf.end(), message.begin(), message.end());
  buf.push_back((uint8_t)msg_type);
  send(client_sock, buf.data(), buf.size(), 0);
}

void BranchOffice::DispatchTask(Task task, int client_sock) {
  if (free_vehicles_.empty()) {
    std::cout << "No free vehicles! Bug!!" << std::endl;
    assert(0);
  } else {
    std::cout << "In DispatchTask" << std::endl;
    std::lock_guard<std::mutex> lock(free_vehicles_mutex_);
    Vehicle vehicle = free_vehicles_.front();
    free_vehicles_.pop();
    std::thread vehicle_handle([this, task, client_sock, vehicle]() {
      TaskWork(task, vehicle, client_sock);
    });
    vehicle_handle.detach();
    if (!free_vehicles_.empty()) {
      std::string status = "IDLE";
      SendMessage(client_sock, status, MSG_TYPE::MSG_BRANCH_STATUS);
    }
  }
}

void BranchOffice::TaskWork(Task task, Vehicle vehicle, int client_sock) {
  for (int i = 1; i <= 10; i++) {
    std::string s = "Branch" + std::to_string(branch_id_) + ":   Task " +
                    std::to_string(task.GetTaskID()) + " is in progress, " +
                    std::to_string(i * 10) + "% " + "by Vehicle " +
                    std::to_string(vehicle.GetVehicleID());
    // std::cout << s << " size: " << buf.size() << std::endl; // size = 42
    // for (auto i : buf) {
    //   std::cout << (unsigned int)i << " ";
    // }
    // std::cout << std::endl;
    SendMessage(client_sock, s, MSG_TYPE::TASK_STATUS_IN_PROGRESS);
    sleep(3);
  }
  std::string s = "Branch" + std::to_string(branch_id_) + ":   Task " +
                  std::to_string(task.GetTaskID()) + " is completed";
  SendMessage(client_sock, s, MSG_TYPE::TASK_STATUS_COMPLETED);
  std::lock_guard<std::mutex> lock(free_vehicles_mutex_);
  free_vehicles_.push(vehicle);
}

void BranchOffice::HandleFunction(int client_sock) {
  // 一开始发送空闲状态
  std::vector<uint8_t> buf;
  std::string status = "IDLE";
  SendMessage(client_sock, status, MSG_TYPE::MSG_BRANCH_STATUS);
  // server 给client的消息只有 task 和 plugin 两种
  while (true) {
    std::vector<uint8_t> buffer(MAX_BUFFER_SIZE);
    int bytes_received = recv(client_sock, buffer.data(), MAX_BUFFER_SIZE, 0);
    buffer.resize(bytes_received);
    std::cout << "Client received" << std::endl;
    int msg_type = buffer.back(); // 假设第最后一个字节是消息类型
    buffer.pop_back();
    if (msg_type == static_cast<int>(MSG_TYPE::MSG_TASK)) {
      Task task;
      task.Deserialize(buffer);
      DispatchTask(task, client_sock);
    } else if (msg_type == static_cast<int>(MSG_TYPE::MSG_PLUGIN)) {
      // 处理插件
    } else {
      std::cout << "Client: unknown msg type "
                << std::string(buffer.begin(), buffer.end()) << std::endl;
    }
  }
}