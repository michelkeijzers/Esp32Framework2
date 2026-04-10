#pragma once

#include "IServiceTasks.hpp"

class IStatusTask;

/**
 * Service tasks available on master nodes.
 */
class IMasterServiceTasks : public IServiceTasks
{
public:
    ~IMasterServiceTasks() override = default;

    virtual IStatusTask &getStatusTask() = 0;
};
