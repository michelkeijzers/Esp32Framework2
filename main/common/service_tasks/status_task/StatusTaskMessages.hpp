#pragma once
#include <cstdint>
#include <cstring>

namespace StatusTaskMessages {

constexpr size_t TASK_NAME_MAX_LEN = 32;

enum class Type : uint8_t {
    SET_TASK_NAME = 1,
    TASK_HEARTBEAT = 2,
    ALL_HEARTBEATS_OK = 3,
    HEARTBEAT_TIMEOUT = 4
};

struct SetTaskName {
    int32_t task_id;
    char task_name[TASK_NAME_MAX_LEN];
};

struct TaskHeartbeat {
    int32_t task_id;
};

struct AllHeartbeatsOk {
    // No fields needed
};

struct HeartbeatTimeout {
    int32_t task_id;
    char task_name[TASK_NAME_MAX_LEN];
};

struct MessageEnvelope {
    Type type;
    union {
        SetTaskName setTaskName;
        TaskHeartbeat taskHeartbeat;
        AllHeartbeatsOk allHeartbeatsOk;
        HeartbeatTimeout heartbeatTimeout;
    } data;
};

}  // namespace StatusTaskMessages
