#pragma once

class Task {
  public:
    void UpdateStatus(int status);

  private:
    int task_id_;
    int task_status_;
};