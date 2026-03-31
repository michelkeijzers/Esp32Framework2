#pragma once
#include "IApiPresetValues.hpp"
#include "../../../common/esp/esp_http_server/esp_http_server_if.hpp"
#include "../../../common/esp/esp_error/esp_error_if.hpp"
#include "../../presets_task/IPresetManager.hpp"

class IEspHttpServer;

class ApiPresetValues : public IApiPresetValues {
public:
    ApiPresetValues(IEspHttpServer& espHttpServer, IPresetManager& presetManager);
    ~ApiPresetValues();

    esp_err_t get_preset_values_handler(httpd_req_t *req) override;
    esp_err_t set_preset_value_handler(httpd_req_t *req) override;

private:
    IEspHttpServer& espHttpServer_;
    IPresetManager& presetManager_;
};
