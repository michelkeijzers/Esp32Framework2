#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ApiSystem.hpp"
#include "MockIEspHttpServer.hpp"

using ::testing::Return;
using ::testing::StrEq;

TEST(ApiSystemTest, RebootHandlerSendsEmptyPlainTextResponse) {
    MockIEspHttpServer mockEspHttpServer;
    ApiSystem apiSystem(mockEspHttpServer);
    httpd_req_t request{};

    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("text/plain")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer, httpd_resp_send(&request, StrEq(""), 0))
        .WillOnce(Return(ESP_OK));

    EXPECT_EQ(0, apiSystem.reboot_handler(&request));
}

TEST(ApiSystemTest, FactoryResetHandlerSendsJsonAckResponse) {
    MockIEspHttpServer mockEspHttpServer;
    ApiSystem apiSystem(mockEspHttpServer);
    httpd_req_t request{};

    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer,
                httpd_resp_send(&request, StrEq("{\"ack\":\"ok\"}"), HTTPD_RESP_USE_STRLEN))
        .WillOnce(Return(ESP_OK));

    EXPECT_EQ(0, apiSystem.factory_reset_handler(&request));
}
