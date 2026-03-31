#pragma once
#include <gmock/gmock.h>
#include "project_dmx_controller/web_server_task/apis/IApiPresets.hpp"

class MockIApiPresets : public IApiPresets
{
public:
    MOCK_METHOD(esp_err_t, get_presets_handler,                (httpd_req_t *), (override));
    MOCK_METHOD(esp_err_t, save_preset_handler,                (httpd_req_t *), (override));
    MOCK_METHOD(esp_err_t, get_active_preset_numbers_handler,  (httpd_req_t *), (override));
    MOCK_METHOD(esp_err_t, select_preset_handler,              (httpd_req_t *), (override));
    MOCK_METHOD(esp_err_t, blackout_handler,                   (httpd_req_t *), (override));
    MOCK_METHOD(esp_err_t, move_preset_up_handler,             (httpd_req_t *), (override));
    MOCK_METHOD(esp_err_t, move_preset_down_handler,           (httpd_req_t *), (override));
    MOCK_METHOD(esp_err_t, delete_preset_handler,              (httpd_req_t *), (override));
    MOCK_METHOD(esp_err_t, insert_preset_at_handler,           (httpd_req_t *), (override));
    MOCK_METHOD(esp_err_t, swap_preset_activation_handler,     (httpd_req_t *), (override));
};
