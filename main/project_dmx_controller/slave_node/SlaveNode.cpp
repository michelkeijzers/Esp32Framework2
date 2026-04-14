#include "SlaveNode.hpp"

SlaveNode::SlaveNode(ISlaveBridge &slaveBridge, ISlaveTask &slaveTask,
                     ISlaveServiceTasks &serviceTasks, std::vector<IFunctionTask *> functionTasks)
    : slaveBridge_(slaveBridge),
      slaveTask_(slaveTask),
      serviceTasks_(serviceTasks),
      functionTasks_(std::move(functionTasks)) {}

esp_err_t SlaveNode::init() {
    esp_err_t ret = slaveTask_.init();
    if (ret != ESP_OK) return ret;

    ret = serviceTasks_.init();
    if (ret != ESP_OK) return ret;

    for (auto *functionTask : functionTasks_) {
        ret = functionTask->init();
        if (ret != ESP_OK) return ret;
    }

    return ESP_OK;
}

esp_err_t SlaveNode::start() {
    esp_err_t ret = slaveTask_.start();
    if (ret != ESP_OK) return ret;

    ret = serviceTasks_.start();
    if (ret != ESP_OK) return ret;

    for (auto *functionTask : functionTasks_) {
        ret = functionTask->start();
        if (ret != ESP_OK) return ret;
    }

    return ESP_OK;
}
