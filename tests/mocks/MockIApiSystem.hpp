#pragma once
#include <gmock/gmock.h>
#include "common/web_server_task/apis/IApiSystem.hpp"

class MockIApiSystem : public IApiSystem
{
public:
    MOCK_METHOD(esp_err_t, reboot_handler,        (httpd_req_t *), (override));
    MOCK_METHOD(esp_err_t, factory_reset_handler, (httpd_req_t *), (override));
};
