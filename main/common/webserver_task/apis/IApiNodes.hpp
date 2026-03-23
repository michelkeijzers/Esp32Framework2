#pragma once
#include "../../esp/esp_http_server/esp_http_server_if.hpp"
#include "../../esp/esp_error/esp_error_if.hpp"

class IApiNodes {
public:
    virtual ~IApiNodes() = default;

    virtual esp_err_t get_nodes_info_handler(httpd_req_t *req) = 0;
    virtual esp_err_t save_nodes_info_handler(httpd_req_t *req) = 0;
};
