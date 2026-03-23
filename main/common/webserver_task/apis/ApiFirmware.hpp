#pragma once
#include "IApiFirmware.hpp"
#include "../../esp/esp_http_server/esp_http_server_if.hpp"
#include "../../esp/esp_error/esp_error_if.hpp"

class IEspHttpServer;
class ApiFirmware : public IApiFirmware {
public:
    ApiFirmware(IEspHttpServer& espHttpServer);
    ~ApiFirmware();

    esp_err_t firmware_chunk_handler(httpd_req_t *req) override;
    esp_err_t firmware_finish_handler(httpd_req_t *req) override;

private:
    IEspHttpServer& espHttpServer_;
};
