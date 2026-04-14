#pragma once

#include <cstdint>
#include <string>
#include <utility>

class TaskStaticInfo {
   public:
    TaskStaticInfo() = default;

    TaskStaticInfo(uint8_t taskId, std::string taskName)
        : taskId(taskId), taskName(std::move(taskName)) {}

    uint8_t taskId = 0;
    std::string taskName;
};
