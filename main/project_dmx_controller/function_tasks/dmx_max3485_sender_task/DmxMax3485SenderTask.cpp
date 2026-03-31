#include "DmxMax3485SenderTask.hpp"

#include <cstring>

static constexpr const char *DMX_TASK_NAME       = "dmx_sender_task";
static constexpr uint32_t    DMX_TASK_STACK_SIZE  = 4096;
static constexpr UBaseType_t DMX_TASK_PRIORITY    = 10;

DmxMax3485SenderTask::DmxMax3485SenderTask(IFreeRtosFactory &freeRtosFactory)
    : FunctionTask(freeRtosFactory, DMX_TASK_NAME, DMX_TASK_STACK_SIZE, DMX_TASK_PRIORITY)
{
    memset(dmxFrame_, 0, sizeof(dmxFrame_));
}

esp_err_t DmxMax3485SenderTask::init()
{
    // TODO: configure UART for DMX (250000 baud, 8N2) and set MAX3485
    //       DE/RE pins to transmit mode
    return ESP_OK;
}

esp_err_t DmxMax3485SenderTask::start()
{
    // TODO: create FreeRTOS task that sends the DMX break + data frame
    //       at 44Hz using the UART peripheral
    return ESP_OK;
}

void DmxMax3485SenderTask::setChannelValue(uint16_t channel, uint8_t value)
{
    if (channel < 512)
    {
        dmxFrame_[channel] = value;
    }
}

void DmxMax3485SenderTask::setAllChannelValues(const uint8_t values[512])
{
    memcpy(dmxFrame_, values, sizeof(dmxFrame_));
}
