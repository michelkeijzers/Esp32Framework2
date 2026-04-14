#pragma once
#include <gmock/gmock.h>

#include "common/node/NodesStaticInfo.hpp"
#include "common/web_server_task/apis/IApiNodes.hpp"

class MockIApiNodes : public IApiNodes {
   public:
    MOCK_METHOD(void, set_nodes_static_info, (const NodesStaticInfo &), (override));
    MOCK_METHOD(esp_err_t, get_nodes_info_handler, (httpd_req_t *), (override));
    MOCK_METHOD(esp_err_t, save_nodes_info_handler, (httpd_req_t *), (override));
};
