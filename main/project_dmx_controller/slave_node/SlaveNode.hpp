#pragma once

#include <vector>

#include "../../common/bridge/slave_bridge/ISlaveBridge.hpp"
#include "../../common/function_tasks/IFunctionTask.hpp"
#include "../../common/node/Node.hpp"
#include "../../common/service_tasks/ISlaveServiceTasks.hpp"
#include "ISlaveNode.hpp"
#include "ISlaveTask.hpp"

/**
 * Concrete slave node.
 *
 * Aggregates all slave-side components by reference and orchestrates
 * their initialisation and start-up sequence.
 */
class SlaveNode : public Node, public ISlaveNode {
   public:
    SlaveNode(ISlaveBridge &slaveBridge, ISlaveTask &slaveTask, ISlaveServiceTasks &serviceTasks,
              std::vector<IFunctionTask *> functionTasks);

    ~SlaveNode() override = default;

    esp_err_t init() override;
    esp_err_t start() override;

   private:
    ISlaveBridge &slaveBridge_;
    ISlaveTask &slaveTask_;
    ISlaveServiceTasks &serviceTasks_;
    std::vector<IFunctionTask *> functionTasks_;
};
