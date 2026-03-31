#include "ServiceTasks.hpp"

ServiceTasks::ServiceTasks(IFreeRtosFactory &freeRtosFactory)
    : loggingTask_(freeRtosFactory)
    , statusTask_(freeRtosFactory)
    , otaTask_(freeRtosFactory)
{
}

esp_err_t ServiceTasks::init()
{
    esp_err_t ret = loggingTask_.init();
    if (ret != ESP_OK) return ret;

    ret = statusTask_.init();
    if (ret != ESP_OK) return ret;

    ret = otaTask_.init();
    return ret;
}

esp_err_t ServiceTasks::start()
{
    esp_err_t ret = loggingTask_.start();
    if (ret != ESP_OK) return ret;

    ret = statusTask_.start();
    if (ret != ESP_OK) return ret;

    ret = otaTask_.start();
    return ret;
}

ILoggingTask &ServiceTasks::getLoggingTask()
{
    return loggingTask_;
}

IStatusTask &ServiceTasks::getStatusTask()
{
    return statusTask_;
}

IOtaTask &ServiceTasks::getOtaTask()
{
    return otaTask_;
}
