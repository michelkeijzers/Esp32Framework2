#pragma once

#include <utility>
#include <vector>

#include "NodeStaticInfo.hpp"

class NodesStaticInfo {
   public:
    NodesStaticInfo() = default;

    explicit NodesStaticInfo(std::vector<NodeStaticInfo> nodesStaticInfo)
        : nodesStaticInfo(std::move(nodesStaticInfo)) {}

    void addNodeStaticInfo(const NodeStaticInfo &nodeInfo) { nodesStaticInfo.push_back(nodeInfo); }

    std::vector<NodeStaticInfo> nodesStaticInfo;
};
