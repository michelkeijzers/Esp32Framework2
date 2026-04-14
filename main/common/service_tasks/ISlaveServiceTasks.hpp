#pragma once

#include "IServiceTasks.hpp"

/**
 * Service tasks available on slave nodes.
 */
class ISlaveServiceTasks : public IServiceTasks {
   public:
    ~ISlaveServiceTasks() override = default;
};
