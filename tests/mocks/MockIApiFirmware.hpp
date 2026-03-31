#pragma once
#include <gmock/gmock.h>
#include "common/webserver_task/apis/IApiFirmware.hpp"

class MockIApiFirmware : public IApiFirmware
{
public:
    MOCK_METHOD(esp_err_t, firmware_chunk_handler,  (httpd_req_t *), (override));
    MOCK_METHOD(esp_err_t, firmware_finish_handler, (httpd_req_t *), (override));
};
