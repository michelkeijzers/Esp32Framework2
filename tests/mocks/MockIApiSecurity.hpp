#pragma once
#include <gmock/gmock.h>
#include "common/web_server_task/apis/IApiSecurity.hpp"

class MockIApiSecurity : public IApiSecurity
{
public:
    MOCK_METHOD(esp_err_t, esp_now_key_handler,   (httpd_req_t *), (override));
    MOCK_METHOD(esp_err_t, wifi_password_handler, (httpd_req_t *), (override));
};
