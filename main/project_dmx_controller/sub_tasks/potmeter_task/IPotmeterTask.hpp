#pragma once

#include "../../../common/sub_tasks/ISubTask.hpp"

/**
 * Interface for the potentiometer (ADC) input sub task.
 */
class IPotmeterTask : public ISubTask
{
public:
    virtual ~IPotmeterTask() = default;

    // TODO: define potentiometer-specific interface methods
    //       (e.g., getValue, setChangeCallback, setReadInterval)
};
