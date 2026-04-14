#pragma once

#include "../../../common/function_tasks/IFunctionTask.hpp"

/**
 * Interface for the potentiometer (ADC) input function task.
 */
class IPotmeterTask : public IFunctionTask {
   public:
    virtual ~IPotmeterTask() = default;

    // TODO: define potentiometer-specific interface methods
    //       (e.g., getValue, setChangeCallback, setReadInterval)
};
