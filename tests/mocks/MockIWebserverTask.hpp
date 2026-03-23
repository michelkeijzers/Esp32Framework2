#pragma once
#include <gmock/gmock.h>
#include "IWebserverTask.hpp"

class MockIWebserverTask : public IWebserverTask
{
public:
    MOCK_METHOD(esp_err_t, init, (), (override));
    MOCK_METHOD(esp_err_t, start, (), (override));
    MOCK_METHOD(esp_err_t, stop, (), (override));
};
