#include "MasterNode.hpp"
#include "../slave_node/ISlaveNode.hpp"

MasterNode::MasterNode(
                       IFreeRtosFactory &rtosFactory,
                       IMasterBridge    &masterBridge,
                       IMasterTask      &masterTask,
                       IWebserverTask   &webserverTask,
                       IServiceTasks    &serviceTasks,
                       std::vector<ISubTask *>   subTasks,
                       std::vector<ISlaveNode *> slaveNodes)
    : rtosFactory_(rtosFactory)
    , masterBridge_(masterBridge)
    , masterTask_(masterTask)
    , webserverTask_(webserverTask)
    , serviceTasks_(serviceTasks)
    , subTasks_(std::move(subTasks))
    , slaveNodes_(std::move(slaveNodes))
{
}

esp_err_t MasterNode::init()
{
    esp_err_t ret = masterTask_.init();
    if (ret != ESP_OK) return ret;

    ret = webserverTask_.init();
    if (ret != ESP_OK) return ret;

    ret = serviceTasks_.init();
    if (ret != ESP_OK) return ret;

    for (auto *subTask : subTasks_)
    {
        ret = subTask->init();
        if (ret != ESP_OK) return ret;
    }

    for (auto *slaveNode : slaveNodes_)
    {
        ret = slaveNode->init();
        if (ret != ESP_OK) return ret;
    }

    return ESP_OK;
}

esp_err_t MasterNode::start()
{
    esp_err_t ret = masterTask_.start();
    if (ret != ESP_OK) return ret;

    ret = webserverTask_.start();
    if (ret != ESP_OK) return ret;

    ret = serviceTasks_.start();
    if (ret != ESP_OK) return ret;

    for (auto *subTask : subTasks_)
    {
        ret = subTask->start();
        if (ret != ESP_OK) return ret;
    }

    for (auto *slaveNode : slaveNodes_)
    {
        ret = slaveNode->start();
        if (ret != ESP_OK) return ret;
    }

    return ESP_OK;
}
