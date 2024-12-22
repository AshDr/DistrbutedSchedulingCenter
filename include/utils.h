#pragma once

#include <cstdint>
enum class TASK_STATUS : uint8_t {
    TASK_STATUS_PENDING = 0,
    TASK_STATUS_IN_PROGRESS,
    TASK_STATUS_COMPLETED,
    TASK_STATUS_FAILE
};

enum class MSG_TYPE : uint8_t {
    MSG_STATUS = 0, // client status message
    MSG_TASK,       // client task process message
    MSG_PLUGIN,     // client plugin message
};

