#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ApiLogging.hpp"
#include "MockIEspHttpServer.hpp"

using ::testing::Return;
using ::testing::StrEq;

TEST(ApiLoggingTest, LoggingHandlerSendsEventStreamResponse) {
    MockIEspHttpServer mockEspHttpServer;
    ApiLogging apiLogging(mockEspHttpServer);
    httpd_req_t request{};

    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("text/event-stream")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer,
                httpd_resp_send(&request, StrEq("data: Log line 1\n\n"), HTTPD_RESP_USE_STRLEN))
        .WillOnce(Return(ESP_OK));

    EXPECT_EQ(0, apiLogging.logging_handler(&request));
}
