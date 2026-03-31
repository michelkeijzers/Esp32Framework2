#pragma once
#include <gmock/gmock.h>
#include "project_dmx_controller/webserver_task/apis/IApiConfiguration.hpp"

class MockIApiConfiguration : public IApiConfiguration
{
public:
    MOCK_METHOD(esp_err_t, get_configuration_handler,       (httpd_req_t *), (override));
    MOCK_METHOD(esp_err_t, put_configuration_handler,       (httpd_req_t *), (override));
    MOCK_METHOD(esp_err_t, set_circular_navigation_handler, (httpd_req_t *), (override));
};
