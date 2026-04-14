#pragma once
#include <gmock/gmock.h>

#include "common/web_server_task/apis/IApiStatus.hpp"

class MockIApiStatus : public IApiStatus {
   public:
    MOCK_METHOD(esp_err_t, get_status_handler, (httpd_req_t *), (override));
    MOCK_METHOD(esp_err_t, get_status_stream_handler, (httpd_req_t *), (override));
};
