#pragma once
#include <gmock/gmock.h>

#include "project_dmx_controller/web_server_task/IWebServerTask.hpp"

class MockIWebServerTask : public IWebServerTask {
   public:
    MOCK_METHOD(esp_err_t, init, (), (override));
    MOCK_METHOD(esp_err_t, start, (), (override));
    MOCK_METHOD(esp_err_t, stop, (), (override));
};
