#pragma once
#include <functional>
#include <string>

#include "IEspHttpServer.hpp"

// Real implementation for ESP32 HTTP server
class EspHttpServer : public IEspHttpServer {
   public:
    EspHttpServer();
    ~EspHttpServer() override;

    esp_err_t httpd_start(httpd_handle_t *handle, const httpd_config_t *config) override;
    esp_err_t httpd_stop(httpd_handle_t handle) override;

    int httpd_req_recv(httpd_req_t *request, char *buf, size_t buf_len) override;

    esp_err_t httpd_resp_set_type(httpd_req_t *req, const char *type) override;
    esp_err_t httpd_resp_send(httpd_req_t *req, const char *buf, size_t len) override;

    esp_err_t httpd_resp_send_chunk(httpd_req_t *request, const char *buf,
                                    ssize_t buf_len) override;
    esp_err_t httpd_resp_send_404(httpd_req_t *req) override;

    esp_err_t httpd_register_uri_handler(httpd_handle_t handle,
                                         const httpd_uri_t *uri_handler) override;
};
