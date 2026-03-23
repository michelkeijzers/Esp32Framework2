#pragma once
#include <gmock/gmock.h>
#include "ISubTask.hpp"

class MockISubTask : public ISubTask
{
public:
    MOCK_METHOD(esp_err_t, init, (), (override));
    MOCK_METHOD(esp_err_t, start, (), (override));
};
