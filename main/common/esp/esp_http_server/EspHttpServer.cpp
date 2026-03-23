#include "EspHttpServer.hpp"
#include "esp_http_server_if.hpp"


EspHttpServer::EspHttpServer() {}
EspHttpServer::~EspHttpServer() {}

esp_err_t EspHttpServer::httpd_start(httpd_handle_t *handle, const httpd_config_t *config) {
    return httpd_start(handle, config);
}

esp_err_t EspHttpServer::httpd_stop(httpd_handle_t handle) { return httpd_stop(handle); }

int EspHttpServer::httpd_req_recv(httpd_req_t *r, char *buf, size_t buf_len) { return httpd_req_recv(r, buf, buf_len); }

esp_err_t EspHttpServer::httpd_resp_set_type(httpd_req_t *req, const char *type) {
    return httpd_resp_set_type(req, type);
}

esp_err_t EspHttpServer::httpd_resp_send(httpd_req_t *req, const char *buf, size_t len) {
    return httpd_resp_send(req, buf, len);
}

esp_err_t EspHttpServer::httpd_resp_send_chunk(httpd_req_t *r, const char *buf, ssize_t buf_len) {
    return httpd_resp_send_chunk(r, buf, buf_len);
}

esp_err_t EspHttpServer::httpd_resp_send_404(httpd_req_t *req) {
    return httpd_resp_send_404(req);
}

esp_err_t EspHttpServer::httpd_register_uri_handler(httpd_handle_t handle,
                                     const httpd_uri_t *uri_handler) {
    return httpd_register_uri_handler(handle, uri_handler);
}   
