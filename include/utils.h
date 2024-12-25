#pragma once

#include <cstdint>

enum class MSG_TYPE : uint8_t {
  MSG_BRANCH_STATUS = 1, // client status message
  MSG_TASK,              // client task process message
  MSG_PLUGIN,            // client plugin message
  TASK_STATUS_PENDING,
  TASK_STATUS_IN_PROGRESS,
  TASK_STATUS_COMPLETED,
  TASK_STATUS_FAILE
};
