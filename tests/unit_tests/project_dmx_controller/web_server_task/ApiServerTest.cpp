#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "MockCommonApiFactory.hpp"
#include "MockEspFactory.hpp"
#include "common/node/NodesStaticInfo.hpp"

#define private public
#define protected public
#include "project_dmx_controller/web_server_task/ApiServer.hpp"
#undef protected
#undef private

using ::testing::_;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgPointee;
using ::testing::StrEq;

class ApiServerTest : public ::testing::Test {
   protected:
    MockEspFactory mockEspFactory;
    MockCommonApiFactory mockCommonApiFactory;
    NodesStaticInfo nodesStaticInfo{std::vector<NodeStaticInfo>{NodeStaticInfo(1, "Node", "fw", "cfg",
                                                                               "aa:bb", "1.2.3.4")}};
};

TEST_F(ApiServerTest, ConstructorPassesNodesInfoToApiNodes) {
    EXPECT_CALL(mockCommonApiFactory.apiNodes_, set_nodes_static_info(_)).Times(1);

    ApiServer apiServer(mockEspFactory, mockCommonApiFactory, nodesStaticInfo);
}

TEST_F(ApiServerTest, StartMountsFileSystemStartsServerAndRegistersEndpoints) {
    EXPECT_CALL(mockCommonApiFactory.apiNodes_, set_nodes_static_info(_)).Times(1);
    ApiServer apiServer(mockEspFactory, mockCommonApiFactory, nodesStaticInfo);
    std::vector<std::string> registeredUris;
    auto serverHandle = reinterpret_cast<httpd_handle_t>(0x44);

    EXPECT_CALL(mockEspFactory.littleFs_, esp_littlefs_mount(_)).WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspFactory.httpServer_, httpd_start(_, _))
        .WillOnce(DoAll(SetArgPointee<0>(serverHandle), Return(ESP_OK)));
    EXPECT_CALL(mockEspFactory.httpServer_, httpd_register_uri_handler(serverHandle, _))
        .Times(12)
        .WillRepeatedly([&](httpd_handle_t, const httpd_uri_t *uri) {
            registeredUris.emplace_back(uri->uri);
            return ESP_OK;
        });

    apiServer.start();

    EXPECT_NE(std::find(registeredUris.begin(), registeredUris.end(), "/api/v1/status"),
              registeredUris.end());
    EXPECT_NE(std::find(registeredUris.begin(), registeredUris.end(), "/api/v1/logging"),
              registeredUris.end());
    EXPECT_NE(std::find(registeredUris.begin(), registeredUris.end(), "/*"),
              registeredUris.end());

    apiServer.stop();
}

TEST_F(ApiServerTest, StartLogsErrorWhenHttpServerStartFails) {
    EXPECT_CALL(mockCommonApiFactory.apiNodes_, set_nodes_static_info(_)).Times(1);
    ApiServer apiServer(mockEspFactory, mockCommonApiFactory, nodesStaticInfo);

    EXPECT_CALL(mockEspFactory.littleFs_, esp_littlefs_mount(_)).WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspFactory.httpServer_, httpd_start(_, _)).WillOnce(Return(ESP_FAIL));
    EXPECT_CALL(mockEspFactory.logger_, log_error(StrEq("ApiServer"), StrEq("Failed to start HTTP server")))
        .Times(1);
    EXPECT_CALL(mockEspFactory.httpServer_, httpd_register_uri_handler(_, _)).Times(0);

    apiServer.start();
}

TEST_F(ApiServerTest, StopStopsServerWhenHandleIsPresent) {
    EXPECT_CALL(mockCommonApiFactory.apiNodes_, set_nodes_static_info(_)).Times(1);
    ApiServer apiServer(mockEspFactory, mockCommonApiFactory, nodesStaticInfo);
    auto serverHandle = reinterpret_cast<httpd_handle_t>(0x55);
    apiServer.server = serverHandle;

    EXPECT_CALL(mockEspFactory.httpServer_, httpd_stop(serverHandle)).Times(1);

    apiServer.stop();
}

TEST_F(ApiServerTest, StaticFileHandlerServesIndexFileForRootUri) {
    EXPECT_CALL(mockCommonApiFactory.apiNodes_, set_nodes_static_info(_)).Times(1);
    ApiServer apiServer(mockEspFactory, mockCommonApiFactory, nodesStaticInfo);
    httpd_req_t request{};
    request.uri = "/";
    std::string body;

    std::filesystem::create_directories("/littlefs");
    {
        std::ofstream out("/littlefs/index.html");
        out << "hello world";
    }

    EXPECT_CALL(mockEspFactory.httpServer_, httpd_resp_set_type(&request, StrEq("text/html")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspFactory.httpServer_, httpd_resp_send_chunk(&request, _, _))
        .WillRepeatedly([&](httpd_req_t *, const char *chunk, ssize_t len) {
            if (chunk != nullptr && len > 0) {
                body.append(chunk, static_cast<std::size_t>(len));
            }
            return ESP_OK;
        });

    EXPECT_EQ(ESP_OK, apiServer.static_file_handler(&request));
    EXPECT_EQ("hello world", body);

    std::filesystem::remove_all("/littlefs");
}

TEST_F(ApiServerTest, StaticFileHandlerReturns404WhenFileAndFallbackAreMissing) {
    EXPECT_CALL(mockCommonApiFactory.apiNodes_, set_nodes_static_info(_)).Times(1);
    ApiServer apiServer(mockEspFactory, mockCommonApiFactory, nodesStaticInfo);
    httpd_req_t request{};
    request.uri = "/missing.js";

    std::filesystem::remove_all("/littlefs");

    EXPECT_CALL(mockEspFactory.httpServer_, httpd_resp_send_404(&request)).Times(1)
        .WillOnce(Return(ESP_OK));

    EXPECT_EQ(ESP_OK, apiServer.static_file_handler(&request));
}
