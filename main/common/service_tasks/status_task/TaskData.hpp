#pragma once

#include <cstdint>
#include <cstring>

#include "free_rtos_if.hpp"

namespace {
constexpr std::size_t TASK_NAME_BUFFER_LENGTH = 32U;
}

class TaskData {
   public:
    TaskData(uint32_t taskId, const char* name, TickType_t lastHeartbeatTick)
        : id_(taskId), lastHeartbeatTick_(lastHeartbeatTick) {
        strncpy(name_, name, sizeof(name_) - 1);
        name_[sizeof(name_) - 1] = '\0';  // Ensure null-termination
    }

    uint32_t getId() const { return id_; }

    const char* getName() const { return name_; }

    TickType_t getLastHeartbeatTick() const { return lastHeartbeatTick_; }

    void updateHeartbeat(TickType_t tick) { lastHeartbeatTick_ = tick; }

   private:
    uint32_t id_;
    char name_[TASK_NAME_BUFFER_LENGTH];
    TickType_t lastHeartbeatTick_;
};