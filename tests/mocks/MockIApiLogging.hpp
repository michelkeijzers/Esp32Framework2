#pragma once
#include <gmock/gmock.h>

#include "common/web_server_task/apis/IApiLogging.hpp"

class MockIApiLogging : public IApiLogging {
   public:
    MOCK_METHOD(esp_err_t, logging_handler, (httpd_req_t *), (override));
};
