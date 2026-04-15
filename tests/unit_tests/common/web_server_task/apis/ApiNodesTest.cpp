#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "ApiNodes.hpp"
#include "MockIEspHttpServer.hpp"
#include "common/node/NodesStaticInfo.hpp"

using ::testing::_;
using ::testing::Return;
using ::testing::StrEq;

TEST(ApiNodesTest, GetNodesInfoHandlerWithoutStaticInfoReturnsEmptyJsonArray) {
    MockIEspHttpServer mockEspHttpServer;
    ApiNodes apiNodes(mockEspHttpServer);
    httpd_req_t request{};
    std::string responseBody;

    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer, httpd_resp_send(&request, _, HTTPD_RESP_USE_STRLEN))
        .WillOnce([&](httpd_req_t*, const char* body, size_t) {
            responseBody = body;
            return ESP_OK;
        });

    EXPECT_EQ(0, apiNodes.get_nodes_info_handler(&request));
    EXPECT_EQ("[]", responseBody);
}

TEST(ApiNodesTest, GetNodesInfoHandlerSerializesNodesTasksAndEscapesJsonCharacters) {
    MockIEspHttpServer mockEspHttpServer;
    ApiNodes apiNodes(mockEspHttpServer);
    httpd_req_t request{};
    std::string responseBody;

    NodeStaticInfo firstNode(1, "Node \"A\"", "fw\\1", "cfg\"1", "aa:bb\\cc", "10.0.0.1");
    firstNode.addTaskStaticInfo(TaskStaticInfo(7, "Task \"One\""));
    firstNode.addTaskStaticInfo(TaskStaticInfo(8, "Task\\Two"));

    NodeStaticInfo secondNode(2, "NodeB", "fw2", "cfg2", "dd:ee:ff", "10.0.0.2");
    secondNode.addTaskStaticInfo(TaskStaticInfo(9, "TaskThree"));

    NodesStaticInfo nodesStaticInfo;
    nodesStaticInfo.addNodeStaticInfo(firstNode);
    nodesStaticInfo.addNodeStaticInfo(secondNode);
    apiNodes.set_nodes_static_info(nodesStaticInfo);

    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer, httpd_resp_send(&request, _, HTTPD_RESP_USE_STRLEN))
        .WillOnce([&](httpd_req_t*, const char* body, size_t) {
            responseBody = body;
            return ESP_OK;
        });

    EXPECT_EQ(0, apiNodes.get_nodes_info_handler(&request));
    EXPECT_EQ(
        "[{\"id\":1,\"name\":\"Node \\\"A\\\"\",\"mac_address\":\"aa:bb\\\\cc\","
        "\"firmware_version\":\"fw\\\\1\",\"configuration_version\":\"cfg\\\"1\","
        "\"ip_address\":\"10.0.0.1\",\"tasks\":[{\"id\":7,\"name\":\"Task \\\"One\\\"\"},"
        "{\"id\":8,\"name\":\"Task\\\\Two\"}]},{\"id\":2,\"name\":\"NodeB\","
        "\"mac_address\":\"dd:ee:ff\",\"firmware_version\":\"fw2\","
        "\"configuration_version\":\"cfg2\",\"ip_address\":\"10.0.0.2\","
        "\"tasks\":[{\"id\":9,\"name\":\"TaskThree\"}]}]",
        responseBody);
}

TEST(ApiNodesTest, SaveNodesInfoHandlerSendsJsonAckResponse) {
    MockIEspHttpServer mockEspHttpServer;
    ApiNodes apiNodes(mockEspHttpServer);
    httpd_req_t request{};

    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer,
                httpd_resp_send(&request, StrEq("{\"ack\":\"ok\"}"), HTTPD_RESP_USE_STRLEN))
        .WillOnce(Return(ESP_OK));

    EXPECT_EQ(0, apiNodes.save_nodes_info_handler(&request));
}
