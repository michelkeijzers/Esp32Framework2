#pragma once
#include <string>
#include <functional>
#include "../esp_error/esp_error_if.hpp"
#include "esp_http_server_if.hpp"

// Interface for ESP32 HTTP server abstraction
class IEspHttpServer {
public:
    virtual ~IEspHttpServer() = default;

    virtual esp_err_t httpd_start(httpd_handle_t *handle, const httpd_config_t *config) = 0;
    virtual esp_err_t httpd_stop(httpd_handle_t handle) = 0;

    virtual int httpd_req_recv(httpd_req_t *r, char *buf, size_t buf_len) = 0;

    virtual esp_err_t httpd_resp_set_type(httpd_req_t *req, const char *type) = 0;
    virtual esp_err_t httpd_resp_send(httpd_req_t *req, const char *buf, size_t len) = 0;

    virtual esp_err_t httpd_resp_send_chunk(httpd_req_t *r, const char *buf, ssize_t buf_len) = 0;
    virtual esp_err_t httpd_resp_send_404(httpd_req_t *req) = 0;

    virtual esp_err_t httpd_register_uri_handler(httpd_handle_t handle,
                                     const httpd_uri_t *uri_handler) = 0;
};
