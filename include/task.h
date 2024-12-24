#pragma once
#include "serializable.h"

class Task : public Serializable {
  public:
    Task() = default;
    Task(int task_id, int task_status)
        : task_id_(task_id), task_status_(task_status) {
    }
    void UpdateStatus(int status);
    int GetTaskID() {
        return task_id_;
    }
    bool Serialize(std::vector<uint8_t> &buf) override;
    bool Deserialize(std::vector<uint8_t> &buf) override;
    int GetType() override;

  private:
    int task_id_;
    int task_status_;
};