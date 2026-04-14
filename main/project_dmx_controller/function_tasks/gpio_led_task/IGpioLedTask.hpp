#pragma once

#include "../../../common/function_tasks/IFunctionTask.hpp"

/**
 * Interface for the GPIO LED output function task.
 */
class IGpioLedTask : public IFunctionTask {
   public:
    virtual ~IGpioLedTask() = default;

    // TODO: define LED-specific interface methods
    //       (e.g., setColor, setBrightness, setPattern)
};
