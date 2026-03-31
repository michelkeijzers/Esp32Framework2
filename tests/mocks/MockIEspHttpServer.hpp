#pragma once
#include <gmock/gmock.h>
#include "IEspHttpServer.hpp"

class MockIEspHttpServer : public IEspHttpServer
{
public:
    MOCK_METHOD(esp_err_t, httpd_start, (httpd_handle_t *, const httpd_config_t *), (override));
    MOCK_METHOD(esp_err_t, httpd_stop, (httpd_handle_t), (override));
    MOCK_METHOD(int, httpd_req_recv, (httpd_req_t *, char *, size_t), (override));
    MOCK_METHOD(esp_err_t, httpd_resp_set_type, (httpd_req_t *, const char *), (override));
    MOCK_METHOD(esp_err_t, httpd_resp_send, (httpd_req_t *, const char *, size_t), (override));
    MOCK_METHOD(esp_err_t, httpd_resp_send_chunk, (httpd_req_t *, const char *, ssize_t), (override));
    MOCK_METHOD(esp_err_t, httpd_resp_send_404, (httpd_req_t *), (override));
    MOCK_METHOD(esp_err_t, httpd_register_uri_handler, (httpd_handle_t, const httpd_uri_t *), (override));
};
