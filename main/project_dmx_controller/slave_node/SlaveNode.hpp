#pragma once

#include "ISlaveNode.hpp"
#include "ISlaveTask.hpp"
#include "../../common/node/Node.hpp"
#include "../../common/bridge/slave_bridge/ISlaveBridge.hpp"
#include "../../common/service_tasks/IServiceTasks.hpp"
#include "../../common/sub_tasks/ISubTask.hpp"

#include <vector>

/**
 * Concrete slave node.
 *
 * Aggregates all slave-side components by reference and orchestrates
 * their initialisation and start-up sequence.
 */
class SlaveNode : public Node, public ISlaveNode
{
public:
    SlaveNode(ISlaveBridge   &slaveBridge,
              ISlaveTask     &slaveTask,
              IServiceTasks  &serviceTasks,
              std::vector<ISubTask *> subTasks);

    ~SlaveNode() override = default;

    esp_err_t init()  override;
    esp_err_t start() override;

private:
    ISlaveBridge  &slaveBridge_;
    ISlaveTask    &slaveTask_;
    IServiceTasks &serviceTasks_;
    std::vector<ISubTask *> subTasks_;
};
