#pragma once

#include "IPotmeterTask.hpp"
#include "../../../common/sub_tasks/SubTask.hpp"

/**
 * Concrete potentiometer (ADC) input sub task.
 *
 * Periodically samples an ADC channel connected to a potentiometer and
 * reports value changes.
 */
class PotmeterTask : public SubTask, public IPotmeterTask
{
public:
    explicit PotmeterTask(int adcChannel);
    ~PotmeterTask() override = default;

    esp_err_t init()  override;
    esp_err_t start() override;

private:
    int adcChannel_;
};
