#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ApiSecurity.hpp"
#include "MockIEspHttpServer.hpp"

using ::testing::Return;
using ::testing::StrEq;

TEST(ApiSecurityTest, EspNowKeyHandlerSendsJsonAckResponse) {
    MockIEspHttpServer mockEspHttpServer;
    ApiSecurity apiSecurity(mockEspHttpServer);
    httpd_req_t request{};

    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer,
                httpd_resp_send(&request, StrEq("{\"ack\":\"ok\"}"), HTTPD_RESP_USE_STRLEN))
        .WillOnce(Return(ESP_OK));

    EXPECT_EQ(0, apiSecurity.esp_now_key_handler(&request));
}

TEST(ApiSecurityTest, WifiPasswordHandlerSendsJsonAckResponse) {
    MockIEspHttpServer mockEspHttpServer;
    ApiSecurity apiSecurity(mockEspHttpServer);
    httpd_req_t request{};

    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer,
                httpd_resp_send(&request, StrEq("{\"ack\":\"ok\"}"), HTTPD_RESP_USE_STRLEN))
        .WillOnce(Return(ESP_OK));

    EXPECT_EQ(0, apiSecurity.wifi_password_handler(&request));
}
