#pragma once
#include "task.h"
#include <string>
class TaskMigrator {
  public:
    std::string SerializeTask();
    Task DeserializeTask(std::string task_str);

  private:
    int MigrationTaskID;
};