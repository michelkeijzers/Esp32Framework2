#pragma once

#include "../../../common/function_tasks/IFunctionTask.hpp"

/**
 * Interface for the button input function task.
 */
class IButtonTask : public IFunctionTask
{
public:
    virtual ~IButtonTask() = default;

    // TODO: define button-specific interface methods
    //       (e.g., setButtonPressCallback, getButtonState)
};
