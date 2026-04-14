#include "MasterNode.hpp"

#include "../slave_node/ISlaveNode.hpp"

MasterNode::MasterNode(IFreeRtosFactory &rtosFactory, IMasterBridge &masterBridge,
                       IMasterTask &masterTask, IWebServerTask &webServerTask,
                       IMasterServiceTasks &serviceTasks,
                       std::vector<IFunctionTask *> functionTasks,
                       std::vector<ISlaveNode *> slaveNodes,
                       std::vector<TaskStatusInfo *> taskStatusInfo)
    : rtosFactory_(rtosFactory),
      masterBridge_(masterBridge),
      masterTask_(masterTask),
      webServerTask_(webServerTask),
      serviceTasks_(serviceTasks),
      functionTasks_(std::move(functionTasks)),
      slaveNodes_(std::move(slaveNodes)) {}

esp_err_t MasterNode::init() {
    esp_err_t ret = masterTask_.init();
    if (ret != ESP_OK) return ret;

    ret = webServerTask_.init();
    if (ret != ESP_OK) return ret;

    ret = serviceTasks_.init();
    if (ret != ESP_OK) return ret;

    for (auto *functionTask : functionTasks_) {
        ret = functionTask->init();
        if (ret != ESP_OK) return ret;
    }

    for (auto *slaveNode : slaveNodes_) {
        ret = slaveNode->init();
        if (ret != ESP_OK) return ret;
    }

    return ESP_OK;
}

esp_err_t MasterNode::start() {
    esp_err_t ret = masterTask_.start();
    if (ret != ESP_OK) return ret;

    ret = webServerTask_.start();
    if (ret != ESP_OK) return ret;

    ret = serviceTasks_.start();
    if (ret != ESP_OK) return ret;

    for (auto *functionTask : functionTasks_) {
        ret = functionTask->start();
        if (ret != ESP_OK) return ret;
    }

    for (auto *slaveNode : slaveNodes_) {
        ret = slaveNode->start();
        if (ret != ESP_OK) return ret;
    }

    return ESP_OK;
}
