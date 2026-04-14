#pragma once

#include "../../../common/function_tasks/FunctionTask.hpp"
#include "IPotmeterTask.hpp"

/**
 * Concrete potentiometer (ADC) input function task.
 *
 * Periodically samples an ADC channel connected to a potentiometer and
 * reports value changes.
 */
class PotmeterTask : public FunctionTask, public IPotmeterTask {
   public:
    PotmeterTask(IFreeRtosFactory &freeRtosFactory, int adcChannel);
    ~PotmeterTask() override = default;

    esp_err_t init() override;
    esp_err_t start() override;

   private:
    int adcChannel_;
};
