#pragma once

#include "IDmxMax3485SenderTask.hpp"
#include "../../../common/sub_tasks/SubTask.hpp"

#include <cstdint>

/**
 * Concrete DMX MAX3485 sender sub task.
 *
 * Continuously transmits DMX512 frames at the required 44Hz cadence over
 * the UART->MAX3485 RS-485 interface.
 */
class DmxMax3485SenderTask : public SubTask, public IDmxMax3485SenderTask
{
public:
    explicit DmxMax3485SenderTask(IFreeRtosFactory &freeRtosFactory);
    ~DmxMax3485SenderTask() override = default;

    esp_err_t init()  override;
    esp_err_t start() override;

    void setChannelValue(uint16_t channel, uint8_t value)    override;
    void setAllChannelValues(const uint8_t values[512])      override;

private:
    uint8_t dmxFrame_[512];
};
