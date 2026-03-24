#pragma once

#include "IServiceTasks.hpp"
#include "logging_task/LoggingTask.hpp"
#include "status_task/StatusTask.hpp"
#include "ota_task/OtaTask.hpp"

class IFreeRtosFactory;

/**
 * Concrete container that creates and manages all common service tasks.
 */
class ServiceTasks : public IServiceTasks
{
public:
    explicit ServiceTasks(IFreeRtosFactory &freeRtosFactory);
    ~ServiceTasks() override = default;

    esp_err_t init()  override;
    esp_err_t start() override;

    ILoggingTask &getLoggingTask() override;
    IStatusTask  &getStatusTask()  override;
    IOtaTask     &getOtaTask()     override;

private:
    LoggingTask loggingTask_;
    StatusTask  statusTask_;
    OtaTask     otaTask_;
};
