#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "TaskStaticInfo.hpp"

class NodeStaticInfo {
   public:
    NodeStaticInfo() = default;

    NodeStaticInfo(uint8_t nodeId, std::string nodeName, std::string firmwareVersion,
                   std::string configurationVersion, std::string macAddress,
                   std::string ipAddress = "")
        : nodeId(nodeId),
          nodeName(std::move(nodeName)),
          firmwareVersion(std::move(firmwareVersion)),
          configurationVersion(std::move(configurationVersion)),
          macAddress(std::move(macAddress)),
          ipAddress(std::move(ipAddress)) {}

    void addTaskStaticInfo(const TaskStaticInfo &taskInfo) { tasksStaticInfo.push_back(taskInfo); }

    uint8_t nodeId = 0;
    std::string nodeName;
    std::string firmwareVersion;
    std::string configurationVersion;
    std::string macAddress;
    std::string ipAddress;
    std::vector<TaskStaticInfo> tasksStaticInfo;
};
