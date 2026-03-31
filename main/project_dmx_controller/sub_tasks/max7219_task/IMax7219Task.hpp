#pragma once

#include "../../../common/sub_tasks/ISubTask.hpp"

/**
 * Interface for the MAX7219 LED matrix / 7-segment display sub task.
 */
class IMax7219Task : public ISubTask
{
public:
    virtual ~IMax7219Task() = default;

    // TODO: define MAX7219-specific interface methods
    //       (e.g., displayNumber, setIntensity, clearDisplay)
};
