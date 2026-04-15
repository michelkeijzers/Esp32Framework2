#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ApiFirmware.hpp"
#include "MockIEspHttpServer.hpp"

using ::testing::Return;
using ::testing::StrEq;

TEST(ApiFirmwareTest, FirmwareChunkHandlerSendsJsonAckResponse) {
    MockIEspHttpServer mockEspHttpServer;
    ApiFirmware apiFirmware(mockEspHttpServer);
    httpd_req_t request{};

    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer,
                httpd_resp_send(&request, StrEq("{\"ack\":\"ok\"}"), HTTPD_RESP_USE_STRLEN))
        .WillOnce(Return(ESP_OK));

    EXPECT_EQ(0, apiFirmware.firmware_chunk_handler(&request));
}

TEST(ApiFirmwareTest, FirmwareFinishHandlerSendsJsonAckResponse) {
    MockIEspHttpServer mockEspHttpServer;
    ApiFirmware apiFirmware(mockEspHttpServer);
    httpd_req_t request{};

    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer,
                httpd_resp_send(&request, StrEq("{\"ack\":\"ok\"}"), HTTPD_RESP_USE_STRLEN))
        .WillOnce(Return(ESP_OK));

    EXPECT_EQ(0, apiFirmware.firmware_finish_handler(&request));
}
