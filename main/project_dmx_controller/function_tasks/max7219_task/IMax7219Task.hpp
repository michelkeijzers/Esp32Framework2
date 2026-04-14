#pragma once

#include "../../../common/function_tasks/IFunctionTask.hpp"

/**
 * Interface for the MAX7219 LED matrix / 7-segment display function task.
 */
class IMax7219Task : public IFunctionTask {
   public:
    virtual ~IMax7219Task() = default;

    // TODO: define MAX7219-specific interface methods
    //       (e.g., displayNumber, setIntensity, clearDisplay)
};
