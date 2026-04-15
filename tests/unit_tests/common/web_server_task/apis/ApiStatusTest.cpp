#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "ApiStatus.hpp"
#include "MockIEspHttpServer.hpp"

using ::testing::_;
using ::testing::HasSubstr;
using ::testing::Return;
using ::testing::StrEq;

class ApiStatusTest : public ::testing::Test {
   protected:
    MockIEspHttpServer mockEspHttpServer;
    ApiStatus apiStatus{mockEspHttpServer};
    httpd_req_t request{};
};

TEST_F(ApiStatusTest, GetStatusHandlerSetsJsonContentTypeAndSendsPayload) {
    std::string body;

    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer, httpd_resp_send(&request, _, HTTPD_RESP_USE_STRLEN))
        .WillOnce([&](httpd_req_t*, const char* payload, size_t) {
            body = payload;
            return ESP_OK;
        });

    EXPECT_EQ(ESP_OK, apiStatus.get_status_handler(&request));
    EXPECT_THAT(body, HasSubstr("\"name\":\"Master\""));
    EXPECT_THAT(body, HasSubstr("\"status_watchdog\":\"OK\""));
}

TEST_F(ApiStatusTest, GetStatusStreamHandlerSetsSseContentTypeAndSendsEvent) {
    std::string body;

    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("text/event-stream")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer, httpd_resp_send(&request, _, HTTPD_RESP_USE_STRLEN))
        .WillOnce([&](httpd_req_t*, const char* payload, size_t) {
            body = payload;
            return ESP_OK;
        });

    EXPECT_EQ(ESP_OK, apiStatus.get_status_stream_handler(&request));
    EXPECT_THAT(body, HasSubstr("data: "));
    EXPECT_THAT(body, HasSubstr("\"node_type\":\"WebServer\""));
    EXPECT_THAT(body, HasSubstr("\n\n"));
}
