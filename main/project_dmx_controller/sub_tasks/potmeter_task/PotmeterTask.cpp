#include "PotmeterTask.hpp"

static constexpr const char *POTMETER_TASK_NAME       = "potmeter_task";
static constexpr uint32_t    POTMETER_TASK_STACK_SIZE  = 2048;
static constexpr UBaseType_t POTMETER_TASK_PRIORITY    = 4;

PotmeterTask::PotmeterTask(IFreeRtosFactory &freeRtosFactory, int adcChannel)
    : SubTask(freeRtosFactory, POTMETER_TASK_NAME, POTMETER_TASK_STACK_SIZE, POTMETER_TASK_PRIORITY)
    , adcChannel_(adcChannel)
{
}

esp_err_t PotmeterTask::init()
{
    // TODO: configure ADC channel and attenuation
    return ESP_OK;
}

esp_err_t PotmeterTask::start()
{
    // TODO: create FreeRTOS task that periodically samples the ADC and
    //       fires callbacks on value change
    return ESP_OK;
}
