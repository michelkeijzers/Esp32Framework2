#pragma once
#include <gmock/gmock.h>
#include "common/webserver_task/apis/IApiNodes.hpp"

class MockIApiNodes : public IApiNodes
{
public:
    MOCK_METHOD(esp_err_t, get_nodes_info_handler,  (httpd_req_t *), (override));
    MOCK_METHOD(esp_err_t, save_nodes_info_handler, (httpd_req_t *), (override));
};
