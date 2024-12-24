#include "branchoffice.h"
#include "utils.h"
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <mutex>
#include <thread>

BranchOffice::BranchOffice(int server_port, char *server_ip, int vehicle_cnt):TCPClient(server_port, server_ip),total_vehicles(vehicle_cnt){
    // std::lock_guard<std::mutex> lock()
    for(int i = 1; i <= vehicle_cnt; i++) {
        free_vehicles_.emplace(i, 0);
    }
}

void BranchOffice::DispatchTask(Task task, int client_sock) {
    if (free_vehicles_.empty()) {
        std::cout << "No free vehicles! Bug!!" << std::endl;
        assert(0);
    } else {
        std::cout << "In DispatchTask" << std::endl;
        Vehicle vehicle = free_vehicles_.front();
        free_vehicles_.pop();
        std::thread vehicle_handle([this, task, client_sock]() {
            TaskWork(task, client_sock);
        });
        vehicle_handle.detach();
    }
}

void BranchOffice::TaskWork(Task task, int client_sock) {
    for (int i = 1; i <= 10; i++) {
        std::string s = "Branch" + std::to_string(branch_id_) + ":   Task " +
                        std::to_string(task.GetTaskID()) + " is in progress, " +
                        std::to_string(i * 10) + "%";
        std::vector<uint8_t> buf;
        buf.insert(buf.end(), s.begin(), s.end()); //
        buf.push_back((int)MSG_TYPE::MSG_TASK);
        std::cout << s << " size: " << buf.size() << std::endl;// size = 42
        send(client_sock, buf.data(), buf.size(), 0); //返回任务状态，在调度中心处理
        sleep(3);
    }

    // 返回任务完成 并将车辆恢复
}

void BranchOffice::HandleFunction(int client_sock) {
    // 一开始发送空闲状态
    std::vector<uint8_t> buf;
    std::string status = "IDLE";
    buf.insert(buf.end(), status.begin(), status.end());
    buf.push_back((uint8_t)MSG_TYPE::MSG_STATUS);
    send(client_sock, buf.data(), buf.size(), 0);
    // server 给client的消息只有 task 和 plugin 两种
    std::vector<uint8_t> buffer(MAX_BUFFER_SIZE);
    while (true) {
        int bytes_received =
            recv(client_sock, buffer.data(), MAX_BUFFER_SIZE, 0);
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
            std::cout << "Client: unknown msg type" << std::endl;
        }
    }
}