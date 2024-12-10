#pragma once

#include "task.h"
#include "vehicle.h"
#include <queue>
#include <vector>

class BranchOffice {
  public:
    void AddTask(Task *task);
    void DispatchTask();
    void ReportTaskCompletion(Task *task);

  private:
    std::vector<Vehicle *> vehicles_;
    std::queue<Task *> tasks_;
};