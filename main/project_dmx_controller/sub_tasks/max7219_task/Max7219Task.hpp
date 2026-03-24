#pragma once

#include "IMax7219Task.hpp"
#include "../../../common/sub_tasks/SubTask.hpp"

/**
 * Concrete MAX7219 LED matrix / 7-segment display driver sub task.
 *
 * Communicates with a MAX7219 chip over SPI to drive LED matrices or
 * 7-segment displays.
 */
class Max7219Task : public SubTask, public IMax7219Task
{
public:
    explicit Max7219Task(IFreeRtosFactory &freeRtosFactory);
    ~Max7219Task() override = default;

    esp_err_t init()  override;
    esp_err_t start() override;
};
