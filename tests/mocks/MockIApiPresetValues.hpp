#pragma once
#include <gmock/gmock.h>
#include "project_dmx_controller/webserver_task/apis/IApiPresetValues.hpp"

class MockIApiPresetValues : public IApiPresetValues
{
public:
    MOCK_METHOD(esp_err_t, get_preset_values_handler, (httpd_req_t *), (override));
    MOCK_METHOD(esp_err_t, set_preset_value_handler,  (httpd_req_t *), (override));
};
