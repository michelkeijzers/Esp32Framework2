#pragma once
#include <gmock/gmock.h>
#include "ISlaveTask.hpp"

class MockISlaveTask : public ISlaveTask
{
public:
    MOCK_METHOD(esp_err_t, init, (), (override));
    MOCK_METHOD(esp_err_t, start, (), (override));
};
