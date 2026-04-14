#pragma once

#include "ISlaveServiceTasks.hpp"
#include "logging_task/LoggingTask.hpp"
#include "ota_task/OtaTask.hpp"

class IFreeRtosFactory;
class NodeStaticInfo;

/**
 * Concrete container that creates and manages shared service tasks.
 */
class ServiceTasks : public ISlaveServiceTasks {
   public:
    ServiceTasks(IFreeRtosFactory &freeRtosFactory, NodeStaticInfo &nodeStaticInfo, int nodeOffset);
    ~ServiceTasks() override = default;

    esp_err_t init() override;
    esp_err_t start() override;

    ILoggingTask &getLoggingTask() override;
    IOtaTask &getOtaTask() override;

   private:
    LoggingTask loggingTask_;
    OtaTask otaTask_;
};
