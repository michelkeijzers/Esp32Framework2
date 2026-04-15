#include "common/esp/esp_http_server/EspHttpServer.hpp"

EspHttpServer::EspHttpServer() = default;
EspHttpServer::~EspHttpServer() = default;

esp_err_t EspHttpServer::httpd_start(httpd_handle_t *handle, const httpd_config_t *) {
    if (handle != nullptr) {
        *handle = nullptr;
    }
    return ESP_OK;
}

esp_err_t EspHttpServer::httpd_stop(httpd_handle_t) { return ESP_OK; }

int EspHttpServer::httpd_req_recv(httpd_req_t *, char *, size_t) { return 0; }

esp_err_t EspHttpServer::httpd_resp_set_type(httpd_req_t *, const char *) { return ESP_OK; }

esp_err_t EspHttpServer::httpd_resp_send(httpd_req_t *, const char *, size_t) { return ESP_OK; }

esp_err_t EspHttpServer::httpd_resp_send_chunk(httpd_req_t *, const char *, ssize_t) {
    return ESP_OK;
}

esp_err_t EspHttpServer::httpd_resp_send_404(httpd_req_t *) { return ESP_OK; }

esp_err_t EspHttpServer::httpd_register_uri_handler(httpd_handle_t, const httpd_uri_t *) {
    return ESP_OK;
}
