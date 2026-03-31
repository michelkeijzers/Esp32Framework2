#pragma once

#include "IMasterNode.hpp"
#include "IMasterTask.hpp"
#include "../../common/node/Node.hpp"
#include "../../common/bridge/master_bridge/IMasterBridge.hpp"
#include "../webserver_task/IWebserverTask.hpp"
#include "../../common/service_tasks/IServiceTasks.hpp"
#include "../../common/sub_tasks/ISubTask.hpp"

#include <vector>
#include "../../common/esp/free_rtos/IFreeRtosFactory.hpp"

// Forward declaration to avoid circular includes
class IFreeRtosFactory;
class ISlaveNode;

/**
 * Concrete master node.
 *
 * Aggregates all top-level components by reference and orchestrates
 * their initialisation and start-up sequence.
 */
class MasterNode : public Node, public IMasterNode
{
public:
    MasterNode(
        IFreeRtosFactory &rtosFactory,
        IMasterBridge    &masterBridge,
               IMasterTask      &masterTask,
               IWebserverTask   &webserverTask,
               IServiceTasks    &serviceTasks,
               std::vector<ISubTask *>   subTasks,
               std::vector<ISlaveNode *> slaveNodes);

    ~MasterNode() override = default;

    esp_err_t init()  override;
    esp_err_t start() override;

private:
    IFreeRtosFactory  &rtosFactory_;
    IMasterBridge  &masterBridge_;
    IMasterTask    &masterTask_;
    IWebserverTask &webserverTask_;
    IServiceTasks  &serviceTasks_;
    std::vector<ISubTask *>   subTasks_;
    std::vector<ISlaveNode *> slaveNodes_;
};
