#pragma once
#include <gmock/gmock.h>
#include "IMasterTask.hpp"

class MockIMasterTask : public IMasterTask
{
public:
    MOCK_METHOD(esp_err_t, init, (), (override));
    MOCK_METHOD(esp_err_t, start, (), (override));
};
