#include "LowLevelStubCounters.hpp"
#include "common/esp/esp_error/esp_error_if.hpp"
#include "common/esp/esp_http_server/EspHttpServer.hpp"

EspHttpServer::EspHttpServer() = default;
EspHttpServer::~EspHttpServer() = default;

esp_err_t EspHttpServer::httpd_start(httpd_handle_t *handle, const httpd_config_t *) {
    ++low_level_stubs::counters().httpd_start_calls;
    if (handle != nullptr) {
        *handle = reinterpret_cast<httpd_handle_t>(0x1);
    }
    return ESP_OK;
}

esp_err_t EspHttpServer::httpd_stop(httpd_handle_t) { return ESP_OK; }

int EspHttpServer::httpd_req_recv(httpd_req_t *, char *, size_t) { return 0; }

esp_err_t EspHttpServer::httpd_resp_set_type(httpd_req_t *req, const char *type) {
    if (req != nullptr && type != nullptr) {
        req->content_type = type;
    }
    return ESP_OK;
}

esp_err_t EspHttpServer::httpd_resp_send(httpd_req_t *req, const char *buf, size_t len) {
    if (req != nullptr && buf != nullptr) {
        req->response.assign(buf, len);
    }
    return ESP_OK;
}

esp_err_t EspHttpServer::httpd_resp_send_chunk(httpd_req_t *req, const char *buf, ssize_t buf_len) {
    if (req != nullptr && buf != nullptr && buf_len > 0) {
        req->response.append(buf, static_cast<size_t>(buf_len));
    }
    return ESP_OK;
}

esp_err_t EspHttpServer::httpd_resp_send_404(httpd_req_t *) { return ESP_OK; }

esp_err_t EspHttpServer::httpd_register_uri_handler(httpd_handle_t, const httpd_uri_t *) {
    return ESP_OK;
}
