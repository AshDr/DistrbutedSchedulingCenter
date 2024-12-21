#pragma once

#include <cstdint>
enum class TASK_STATUS : uint8_t {
    TASK_STATUS_PENDING = 0,
    TASK_STATUS_IN_PROGRESS,
    TASK_STATUS_COMPLETED,
    TASK_STATUS_FAILED
};

enum class MSG_TYPE : uint8_t {
    MSG_STATUS = 1, // client status message
    MSG_TASK,       // client task process message
    MSG_PLUGIN,     // client plugin message
};

enum class CLASS_TYPE : uint8_t {
    TASK = 0,
    PLUGIN,
};