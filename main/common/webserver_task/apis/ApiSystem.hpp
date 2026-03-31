#pragma once
#include "IApiSystem.hpp"
#include "../../esp/esp_http_server/esp_http_server_if.hpp"
#include "../../esp/esp_error/esp_error_if.hpp"

class IEspHttpServer;

class ApiSystem : public IApiSystem {
public:
    ApiSystem(IEspHttpServer& espHttpServer);
    ~ApiSystem();

    esp_err_t reboot_handler(httpd_req_t *req) override;
    esp_err_t factory_reset_handler(httpd_req_t *req) override;

private:
    IEspHttpServer& espHttpServer_;
};
