#pragma once
#include "../../esp/esp_http_server/esp_http_server_if.hpp"
#include "../../esp/esp_error/esp_error_if.hpp"

class IApiFirmware {
public:
    virtual ~IApiFirmware() = default;

    virtual esp_err_t firmware_chunk_handler(httpd_req_t *req) = 0;
    virtual esp_err_t firmware_finish_handler(httpd_req_t *req) = 0;
};
