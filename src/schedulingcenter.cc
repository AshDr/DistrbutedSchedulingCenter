#include "schedulingcenter.h"
#include <iostream>

SchedulingCenter::SchedulingCenter(int server_port, char *bound_ip,
                                   int length_of_queue_of_listen)
    : TCPServer<SchedulingCenter>(server_port, length_of_queue_of_listen,
                                  bound_ip) {
}
SchedulingCenter::~SchedulingCenter() {
    GenerateReport();
}

void SchedulingCenter::AssignTask(int client_sock) {
    std::lock_guard<std::mutex> lock(task_mutex_);
    std::lock_guard<std::mutex> lock_free_clients(free_clients_mutex_);
    free_clients_.push(client_sock);
    while (!tasks_.empty() && !free_clients_.empty()) {
        Task task = tasks_.front();
        tasks_.pop();
        int free_client_sock = free_clients_.front();
        free_clients_.pop();
        std::vector<char> buf;
        task.Serialize(buf);
        send(free_client_sock, buf.data(), buf.size(), 0);
    }
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
    std::vector<char> buffer(MAX_BUFFER_SIZE);
    while (true) {
        int bytes_received =
            recv(client_sock, buffer.data(), MAX_BUFFER_SIZE, 0); // 阻塞
        if (bytes_received <= 0) {
            std::cout << "client " << client_sock << " disconnect" << std::endl;
            client_status_map.erase(client_sock); // 移除该客户端
            close(client_sock);
            break;
        }
        uint8_t msg_type = buffer.back(); // 假设第最后一个字节是消息类型
        buffer.pop_back();
        if (msg_type == MSG_STATUS) {
            HandleStatus(client_sock, buffer);
        } else if (msg_type == MSG_TASK) {
            HandleMonitorTask(client_sock, buffer);
        } else {
            std::cout << "unknown msg type" << std::endl;
        }
    }
}

void SchedulingCenter::HandleStatus(int client_sock,
                                    std::vector<char> &buffer) {
    std::string status(buffer.begin(), buffer.end());
    if (status == "BUSY") {
        client_status_map[client_sock] = BUSY;
    } else if (status == "IDLE") {
        client_status_map[client_sock] = IDLE;
        AssignTask(client_sock); // 其实也可以改成每隔一段时间就调一次
    } else {
        std::cout << "unknown status" << std::endl;
    }
}

void SchedulingCenter::HandleMonitorTask(int client_sock,
                                         std::vector<char> &buffer) {
    std::string task_current_process_report(buffer.begin(), buffer.end());
    std::lock_guard<std::mutex> lock(report_mutex_);
    reports_.push_back(task_current_process_report);
}
