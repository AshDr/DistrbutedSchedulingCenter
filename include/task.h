#pragma once
#include "serializable.h"
enum {
    TASK_STATUS_PENDING = 0,
    TASK_STATUS_IN_PROGRESS,
    TASK_STATUS_COMPLETED,
    TASK_STATUS_FAILED
};
class Task : public Serializable {
  public:
    Task() = default;
    Task(int task_id, int task_status)
        : task_id_(task_id), task_status_(task_status) {
    }
    void UpdateStatus(int status);
    bool Serialize(std::vector<char> &buf) override;
    Serializable *Deserialize(std::vector<char> &buf) override;
    int GetType() override;

  private:
    int task_id_;
    int task_status_;
};