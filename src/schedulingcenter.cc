#include "schedulingcenter.h"
#include "utils.h"
#include <cstdint>
#include <iostream>
#include <mutex>
#include <sys/types.h>

SchedulingCenter::SchedulingCenter(int server_port,
                                   int length_of_queue_of_listen,
                                   char *bound_ip)
    : TCPServer(server_port, length_of_queue_of_listen, bound_ip) {}
SchedulingCenter::~SchedulingCenter() { GenerateReport(); }

void SchedulingCenter::AddTask(int id) {
  std::unique_lock<std::mutex> lock(task_mutex_);
  Task task(id, static_cast<int>(MSG_TYPE::TASK_STATUS_PENDING));
  std::cout << "Addtask " << id << std::endl;
  tasks_.push(task);
  task_cv_.notify_all();
}

template <typename T>
void SchedulingCenter::SendMessage(int client_sock, std::string message,
                               T msg_type) {
  std::vector<uint8_t> buf;
  buf.insert(buf.end(), message.begin(), message.end());
  buf.push_back((uint8_t)msg_type);
  send(client_sock, buf.data(), buf.size(), 0);
}

void SchedulingCenter::AssignTask(int client_sock) {
  std::cout << "AssignTask start1" << std::endl;
  std::unique_lock<std::mutex> lock(task_mutex_);
  std::unique_lock<std::mutex> lock_free_clients(free_clients_mutex_);
  task_cv_.wait(lock, [this] { return !tasks_.empty(); });
  std::cout << "AssignTask start2" << std::endl;
  free_clients_.push(client_sock);

  while (!tasks_.empty() && !free_clients_.empty()) {
    Task task = tasks_.front();
    tasks_.pop();
    int free_client_sock = free_clients_.front();
    free_clients_.pop();
    std::vector<uint8_t> buf;
    task.Serialize(buf);
    buf.push_back(static_cast<char>(MSG_TYPE::MSG_TASK));
    send(free_client_sock, buf.data(), buf.size(), 0);
  }
}

void SchedulingCenter::UpdatePlugin(std::string plugin_name, std::string plugin_dir) {
  std::string plugin_path = plugin_dir + plugin_name;
  for(const auto &[client_sock, status]: client_status_map) {
    SendMessage(client_sock, plugin_path, MSG_TYPE::MSG_PLUGIN);
  };
}

void SchedulingCenter::GenerateReport() {
  // 将结果写入到文件中
  int fd = open("report.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
  if (fd == -1) {
    std::cout << "open file error" << std::endl;
    return;
  }
  // std::lock_guard<std::mutex> lock(report_mutex_);
  for (auto &report : reports_) {
    write(fd, report.c_str(), report.size());
    write(fd, "\n", 1);
  }
}

void SchedulingCenter::HandleFunction(int client_sock) {
  while (true) {
    std::vector<uint8_t> buffer(MAX_BUFFER_SIZE);
    int bytes_received =
        recv(client_sock, buffer.data(), MAX_BUFFER_SIZE, 0); // 阻塞
    if (bytes_received <= 0) {
      std::cout << "client " << client_sock << " disconnect" << std::endl;
      client_status_map.erase(client_sock); // 移除该客户端
      close(client_sock);
      break;
    }

    buffer.resize(bytes_received);
    uint8_t msg_type = buffer.back(); // 最后一个字节是消息类型
    // for (auto x : buffer) {
    //   std::cout << (unsigned int)x << " ";
    // }
    // std::cout << std::endl;
    // std::cout << "buffer size: " << buffer.size() << std::endl;
    std::cout << "msg type " << static_cast<unsigned int>(msg_type) << " end"
              << std::endl; // always 0
    buffer.pop_back();
    if (msg_type == static_cast<uint8_t>(MSG_TYPE::MSG_BRANCH_STATUS)) {
      HandleStatus(client_sock, buffer);
    } else if (msg_type ==
               static_cast<uint8_t>(MSG_TYPE::TASK_STATUS_IN_PROGRESS)) {
      HandleMonitorTask(client_sock, buffer);
    } else if (msg_type ==
               static_cast<uint8_t>(MSG_TYPE::TASK_STATUS_COMPLETED)) {
      HandleMonitorTask(client_sock, buffer);
      AssignTask(client_sock);
    } else if (msg_type == static_cast<uint8_t>(MSG_TYPE::TASK_STATUS_FAILE)) {
      std::cout << "Server: receive task failed msg" << std::endl;
      std::lock_guard<std::mutex> lock(free_clients_mutex_);
      free_clients_.push(client_sock);
    } else if (msg_type == static_cast<uint8_t>(MSG_TYPE::MSG_PLUGIN)) {
      std::cout << "Server: receive plugin msg" << std::endl;
    } else {
      std::cout << "Server: unknown msg type" << std::endl;
    }
  }
}

void SchedulingCenter::HandleStatus(int client_sock,
                                    std::vector<uint8_t> &buffer) {
  std::string status(buffer.begin(), buffer.end());
  if (status == "BUSY") {
    client_status_map[client_sock] = BUSY;
  } else if (status == "IDLE") {
    client_status_map[client_sock] = IDLE;
    AssignTask(client_sock); // 有空闲branch时分配任务
  } else {
    std::cout << "unknown status " << status << std::endl;
  }
}

void SchedulingCenter::HandleMonitorTask(int client_sock,
                                         std::vector<uint8_t> &buffer) {
  std::string task_current_process_report(buffer.begin(), buffer.end());
  std::cout << task_current_process_report << std::endl;
  std::lock_guard<std::mutex> lock(report_mutex_);
  reports_.push_back(task_current_process_report);
}
