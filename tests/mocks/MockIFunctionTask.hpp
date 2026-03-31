#pragma once
#include <gmock/gmock.h>
#include "IFunctionTask.hpp"

class MockIFunctionTask : public IFunctionTask
{
public:
    MOCK_METHOD(esp_err_t, init, (), (override));
    MOCK_METHOD(esp_err_t, start, (), (override));
};
