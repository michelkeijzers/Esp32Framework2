#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstring>
#include <string>

#include "MockIEspHttpServer.hpp"
#include "MockIEspNvs.hpp"
#include "project_dmx_controller/web_server_task/apis/ApiConfiguration.hpp"

using ::testing::_;
using ::testing::DoAll;
using ::testing::HasSubstr;
using ::testing::Return;
using ::testing::SetArgPointee;
using ::testing::StrEq;

class ApiConfigurationTest : public ::testing::Test {
   protected:
    MockIEspHttpServer mockEspHttpServer;
    MockIEspNvs mockNvs;
    ApiConfiguration apiConfiguration{mockEspHttpServer, mockNvs};
    httpd_req_t request{};
    nvs_handle_t nvsHandle{77};
};

TEST_F(ApiConfigurationTest, GetConfigurationReturnsStoredValues) {
    std::string response;

    EXPECT_CALL(mockNvs, nvs_open(StrEq("config"), NVS_READONLY, _))
        .WillOnce(DoAll(SetArgPointee<2>(nvsHandle), Return(ESP_OK)));
    EXPECT_CALL(mockNvs, nvs_get_str(nvsHandle, StrEq("wifi_ssid"), _, _))
        .WillOnce([](nvs_handle_t, const char *, char *out, size_t *) {
            std::strcpy(out, "NodeWifi");
            return ESP_OK;
        });
    EXPECT_CALL(mockNvs, nvs_get_str(nvsHandle, StrEq("device_name"), _, _))
        .WillOnce([](nvs_handle_t, const char *, char *out, size_t *) {
            std::strcpy(out, "node_a");
            return ESP_OK;
        });
    EXPECT_CALL(mockNvs, nvs_get_u32(nvsHandle, StrEq("circ_nav"), _))
        .WillOnce([](nvs_handle_t, const char *, uint32_t *value) {
            *value = 1;
            return ESP_OK;
        });
    EXPECT_CALL(mockNvs, nvs_close(nvsHandle)).Times(1);
    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer,
                httpd_resp_send(&request,
                                StrEq("{\"wifi_ssid\":\"NodeWifi\",\"device_name\":\"node_a\","
                                      "\"circular navigation\":true}"),
                                HTTPD_RESP_USE_STRLEN))
        .WillOnce(Return(ESP_OK));

    EXPECT_EQ(ESP_OK, apiConfiguration.get_configuration_handler(&request));
}

TEST_F(ApiConfigurationTest, GetConfigurationReturnsDefaultsWhenNvsOpenFails) {
    EXPECT_CALL(mockNvs, nvs_open(StrEq("config"), NVS_READONLY, _)).WillOnce(Return(ESP_FAIL));
    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(
        mockEspHttpServer,
        httpd_resp_send(
            &request,
            StrEq("{\"wifi_ssid\":\"\",\"device_name\":\"\",\"circular navigation\":false}"),
            HTTPD_RESP_USE_STRLEN))
        .WillOnce(Return(ESP_OK));

    EXPECT_EQ(ESP_OK, apiConfiguration.get_configuration_handler(&request));
}

TEST_F(ApiConfigurationTest, PutConfigurationReturnsErrorWhenNvsOpenFails) {
    const char *body = "{\"wifi_ssid\":\"A\",\"device_name\":\"B\"}";

    EXPECT_CALL(mockEspHttpServer, httpd_req_recv(&request, _, _))
        .WillOnce([&](httpd_req_t *, char *buffer, size_t) {
            std::strcpy(buffer, body);
            return static_cast<int>(std::strlen(body));
        })
        .WillOnce(Return(0));
    EXPECT_CALL(mockNvs, nvs_open(StrEq("config"), NVS_READWRITE, _)).WillOnce(Return(ESP_FAIL));
    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer,
                httpd_resp_send(&request, StrEq("{\"ack\":\"nok\"}"), HTTPD_RESP_USE_STRLEN))
        .WillOnce(Return(ESP_OK));

    EXPECT_EQ(ESP_FAIL, apiConfiguration.put_configuration_handler(&request));
}

TEST_F(ApiConfigurationTest, PutConfigurationWritesValuesAndAcknowledges) {
    const char *body = "{\"wifi_ssid\":\"DeskWifi\",\"device_name\":\"desk_1\"}";

    EXPECT_CALL(mockEspHttpServer, httpd_req_recv(&request, _, _))
        .WillOnce([&](httpd_req_t *, char *buffer, size_t) {
            std::strcpy(buffer, body);
            return static_cast<int>(std::strlen(body));
        })
        .WillOnce(Return(0));
    EXPECT_CALL(mockNvs, nvs_open(StrEq("config"), NVS_READWRITE, _))
        .WillOnce(DoAll(SetArgPointee<2>(nvsHandle), Return(ESP_OK)));
    EXPECT_CALL(mockNvs, nvs_set_str(nvsHandle, StrEq("wifi_ssid"), StrEq("DeskWifi")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockNvs, nvs_set_str(nvsHandle, StrEq("device_name"), StrEq("desk_1")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockNvs, nvs_commit(nvsHandle)).WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockNvs, nvs_close(nvsHandle)).Times(1);
    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer,
                httpd_resp_send(&request, StrEq("{\"ack\":\"ok\"}"), HTTPD_RESP_USE_STRLEN))
        .WillOnce(Return(ESP_OK));

    EXPECT_EQ(ESP_OK, apiConfiguration.put_configuration_handler(&request));
}

TEST_F(ApiConfigurationTest, SetCircularNavigationStoresTrueAndReturnsResponse) {
    const char *body = "{\"circular navigation\": true}";
    std::string response;

    EXPECT_CALL(mockEspHttpServer, httpd_req_recv(&request, _, _))
        .WillOnce([&](httpd_req_t *, char *buffer, size_t) {
            std::strcpy(buffer, body);
            return static_cast<int>(std::strlen(body));
        })
        .WillOnce(Return(0));
    EXPECT_CALL(mockNvs, nvs_open(StrEq("config"), NVS_READWRITE, _))
        .WillOnce(DoAll(SetArgPointee<2>(nvsHandle), Return(ESP_OK)));
    EXPECT_CALL(mockNvs, nvs_set_u32(nvsHandle, StrEq("circ_nav"), 1u)).WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockNvs, nvs_commit(nvsHandle)).WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockNvs, nvs_close(nvsHandle)).Times(1);
    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer, httpd_resp_send(&request, _, HTTPD_RESP_USE_STRLEN))
        .WillOnce([&](httpd_req_t *, const char *bodyOut, size_t) {
            response = bodyOut;
            return ESP_OK;
        });

    EXPECT_EQ(ESP_OK, apiConfiguration.set_circular_navigation_handler(&request));
    EXPECT_THAT(response, HasSubstr("true"));
}
