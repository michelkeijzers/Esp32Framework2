#pragma once

#include "../../../common/sub_tasks/ISubTask.hpp"

/**
 * Interface for the button input sub task.
 */
class IButtonTask : public ISubTask
{
public:
    virtual ~IButtonTask() = default;

    // TODO: define button-specific interface methods
    //       (e.g., setButtonPressCallback, getButtonState)
};
