#pragma once
#include "../../../common/esp/esp_error/esp_error_if.hpp"
#include "../../../common/esp/esp_http_server/esp_http_server_if.hpp"

class IApiPresets {
   public:
    virtual ~IApiPresets() = default;

    virtual esp_err_t get_presets_handler(httpd_req_t *req) = 0;
    virtual esp_err_t save_preset_handler(httpd_req_t *req) = 0;
    virtual esp_err_t get_active_preset_numbers_handler(httpd_req_t *req) = 0;
    virtual esp_err_t select_preset_handler(httpd_req_t *req) = 0;
    virtual esp_err_t blackout_handler(httpd_req_t *req) = 0;
    virtual esp_err_t move_preset_up_handler(httpd_req_t *req) = 0;
    virtual esp_err_t move_preset_down_handler(httpd_req_t *req) = 0;
    virtual esp_err_t delete_preset_handler(httpd_req_t *req) = 0;
    virtual esp_err_t insert_preset_at_handler(httpd_req_t *req) = 0;
    virtual esp_err_t swap_preset_activation_handler(httpd_req_t *req) = 0;
};
