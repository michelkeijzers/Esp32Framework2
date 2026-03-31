#pragma once
#include <gmock/gmock.h>
#include "project_dmx_controller/webserver_task/IWebserverTask.hpp"

class MockIWebserverTask : public IWebserverTask
{
public:
    MOCK_METHOD(esp_err_t, init, (), (override));
    MOCK_METHOD(esp_err_t, start, (), (override));
    MOCK_METHOD(esp_err_t, stop, (), (override));
};
