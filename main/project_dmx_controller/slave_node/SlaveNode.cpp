#include "SlaveNode.hpp"

SlaveNode::SlaveNode(ISlaveBridge   &slaveBridge,
                     ISlaveTask     &slaveTask,
                     IServiceTasks  &serviceTasks,
                     std::vector<ISubTask *> subTasks)
    : slaveBridge_(slaveBridge)
    , slaveTask_(slaveTask)
    , serviceTasks_(serviceTasks)
    , subTasks_(std::move(subTasks))
{
}

esp_err_t SlaveNode::init()
{
    esp_err_t ret = slaveTask_.init();
    if (ret != ESP_OK) return ret;

    ret = serviceTasks_.init();
    if (ret != ESP_OK) return ret;

    for (auto *subTask : subTasks_)
    {
        ret = subTask->init();
        if (ret != ESP_OK) return ret;
    }

    return ESP_OK;
}

esp_err_t SlaveNode::start()
{
    esp_err_t ret = slaveTask_.start();
    if (ret != ESP_OK) return ret;

    ret = serviceTasks_.start();
    if (ret != ESP_OK) return ret;

    for (auto *subTask : subTasks_)
    {
        ret = subTask->start();
        if (ret != ESP_OK) return ret;
    }

    return ESP_OK;
}
