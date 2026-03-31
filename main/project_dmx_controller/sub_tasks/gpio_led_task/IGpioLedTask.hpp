#pragma once

#include "../../../common/sub_tasks/ISubTask.hpp"

/**
 * Interface for the GPIO LED output sub task.
 */
class IGpioLedTask : public ISubTask
{
public:
    virtual ~IGpioLedTask() = default;

    // TODO: define LED-specific interface methods
    //       (e.g., setColor, setBrightness, setPattern)
};
