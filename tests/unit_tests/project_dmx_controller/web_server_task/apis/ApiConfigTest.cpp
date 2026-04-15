#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstring>
#include <string>

#include "MockIEspHttpServer.hpp"
#include "MockIEspNvs.hpp"
#include "project_dmx_controller/web_server_task/apis/ApiConfig.hpp"

using ::testing::_;
using ::testing::DoAll;
using ::testing::HasSubstr;
using ::testing::Return;
using ::testing::SetArgPointee;
using ::testing::StrEq;

class ApiConfigTest : public ::testing::Test {
   protected:
    MockIEspHttpServer mockEspHttpServer;
    MockIEspNvs mockNvs;
    ApiConfig apiConfig{mockEspHttpServer, mockNvs};
    httpd_req_t request{};
    nvs_handle_t nvsHandle{123};

    static int captureRequestBody(httpd_req_t *, char *buffer, size_t, const char *body) {
        std::strcpy(buffer, body);
        return static_cast<int>(std::strlen(body));
    }
};

TEST_F(ApiConfigTest, GetConfigReturnsErrorResponseWhenNvsOpenFails) {
    std::string response;

    EXPECT_CALL(mockNvs, nvs_open(StrEq("config"), NVS_READONLY, _)).WillOnce(Return(ESP_FAIL));
    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer, httpd_resp_send(&request, _, _))
        .WillOnce([&](httpd_req_t *, const char *body, size_t len) {
            response.assign(body, len);
            return ESP_OK;
        });

    EXPECT_EQ(ESP_OK, apiConfig.get_config_handler(&request));
    EXPECT_THAT(response, HasSubstr("Failed to open NVS"));
    EXPECT_THAT(response, HasSubstr("\"code\":500"));
}

TEST_F(ApiConfigTest, GetConfigReturnsStoredValuesWhenNvsOpenSucceeds) {
    std::string response;

    EXPECT_CALL(mockNvs, nvs_open(StrEq("config"), NVS_READONLY, _))
        .WillOnce(DoAll(SetArgPointee<2>(nvsHandle), Return(ESP_OK)));
    EXPECT_CALL(mockNvs, nvs_get_str(nvsHandle, StrEq("wifi_ssid"), _, _))
        .WillOnce([](nvs_handle_t, const char *, char *out, size_t *) {
            std::strcpy(out, "StudioWifi");
            return ESP_OK;
        });
    EXPECT_CALL(mockNvs, nvs_get_str(nvsHandle, StrEq("device_name"), _, _))
        .WillOnce([](nvs_handle_t, const char *, char *out, size_t *) {
            std::strcpy(out, "dmx_master");
            return ESP_OK;
        });
    EXPECT_CALL(mockNvs, nvs_close(nvsHandle)).Times(1);
    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer, httpd_resp_send(&request, _, _))
        .WillOnce([&](httpd_req_t *, const char *body, size_t len) {
            response.assign(body, len);
            return ESP_OK;
        });

    EXPECT_EQ(ESP_OK, apiConfig.get_config_handler(&request));
    EXPECT_THAT(response, HasSubstr("StudioWifi"));
    EXPECT_THAT(response, HasSubstr("dmx_master"));
    EXPECT_THAT(response, HasSubstr("espnow_configured"));
}

TEST_F(ApiConfigTest, PutConfigRejectsInvalidSsid) {
    std::string response;
    const char *body = "{\"wifi_ssid\":\"\",\"device_name\":\"master\"}";

    EXPECT_CALL(mockEspHttpServer, httpd_req_recv(&request, _, _))
        .WillOnce([&](httpd_req_t *req, char *buffer, size_t maxLen) {
            (void)req;
            (void)maxLen;
            return captureRequestBody(req, buffer, maxLen, body);
        })
        .WillOnce(Return(0));
    EXPECT_CALL(mockNvs, nvs_open(_, _, _)).Times(0);
    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer, httpd_resp_send(&request, _, _))
        .WillOnce([&](httpd_req_t *, const char *bodyOut, size_t) {
            response = bodyOut;
            return ESP_OK;
        });

    EXPECT_EQ(ESP_OK, apiConfig.put_config_handler(&request));
    EXPECT_THAT(response, HasSubstr("Invalid SSID"));
}

TEST_F(ApiConfigTest, PutConfigRejectsInvalidDeviceName) {
    std::string response;
    const char *body = "{\"wifi_ssid\":\"HomeWifi\",\"device_name\":\"invalid name\"}";

    EXPECT_CALL(mockEspHttpServer, httpd_req_recv(&request, _, _))
        .WillOnce([&](httpd_req_t *req, char *buffer, size_t maxLen) {
            (void)req;
            (void)maxLen;
            return captureRequestBody(req, buffer, maxLen, body);
        })
        .WillOnce(Return(0));
    EXPECT_CALL(mockNvs, nvs_open(_, _, _)).Times(0);
    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer, httpd_resp_send(&request, _, _))
        .WillOnce([&](httpd_req_t *, const char *bodyOut, size_t) {
            response = bodyOut;
            return ESP_OK;
        });

    EXPECT_EQ(ESP_OK, apiConfig.put_config_handler(&request));
    EXPECT_THAT(response, HasSubstr("Invalid device name"));
}

TEST_F(ApiConfigTest, PutConfigReturnsErrorWhenNvsOpenFails) {
    std::string response;
    const char *body = "{\"wifi_ssid\":\"StageWifi\",\"device_name\":\"master_1\"}";

    EXPECT_CALL(mockEspHttpServer, httpd_req_recv(&request, _, _))
        .WillOnce([&](httpd_req_t *req, char *buffer, size_t maxLen) {
            (void)req;
            (void)maxLen;
            return captureRequestBody(req, buffer, maxLen, body);
        })
        .WillOnce(Return(0));
    EXPECT_CALL(mockNvs, nvs_open(StrEq("config"), NVS_READWRITE, _)).WillOnce(Return(ESP_FAIL));
    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer, httpd_resp_send(&request, _, _))
        .WillOnce([&](httpd_req_t *, const char *bodyOut, size_t) {
            response = bodyOut;
            return ESP_OK;
        });

    EXPECT_EQ(ESP_OK, apiConfig.put_config_handler(&request));
    EXPECT_THAT(response, HasSubstr("Failed to open NVS"));
}

TEST_F(ApiConfigTest, PutConfigWritesOnlyProvidedDeviceName) {
    std::string response;
    const char *body = "{\"device_name\":\"master_only\"}";

    EXPECT_CALL(mockEspHttpServer, httpd_req_recv(&request, _, _))
        .WillOnce([&](httpd_req_t *req, char *buffer, size_t maxLen) {
            (void)req;
            (void)maxLen;
            return captureRequestBody(req, buffer, maxLen, body);
        })
        .WillOnce(Return(0));
    EXPECT_CALL(mockNvs, nvs_open(StrEq("config"), NVS_READWRITE, _))
        .WillOnce(DoAll(SetArgPointee<2>(nvsHandle), Return(ESP_OK)));
    EXPECT_CALL(mockNvs, nvs_set_str(nvsHandle, StrEq("wifi_ssid"), _)).Times(0);
    EXPECT_CALL(mockNvs, nvs_set_str(nvsHandle, StrEq("device_name"), StrEq("master_only")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockNvs, nvs_commit(nvsHandle)).WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockNvs, nvs_close(nvsHandle)).Times(1);
    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer, httpd_resp_send(&request, _, _))
        .WillOnce([&](httpd_req_t *, const char *bodyOut, size_t len) {
            response.assign(bodyOut, len);
            return ESP_OK;
        });

    EXPECT_EQ(ESP_OK, apiConfig.put_config_handler(&request));
    EXPECT_THAT(response, HasSubstr("master_only"));
}

TEST_F(ApiConfigTest, PutConfigWritesValuesAndReturnsSuccessResponse) {
    std::string response;
    const char *body = "{\"wifi_ssid\":\"StageWifi\",\"device_name\":\"master_1\"}";

    EXPECT_CALL(mockEspHttpServer, httpd_req_recv(&request, _, _))
        .WillOnce([&](httpd_req_t *req, char *buffer, size_t maxLen) {
            (void)req;
            (void)maxLen;
            return captureRequestBody(req, buffer, maxLen, body);
        })
        .WillOnce(Return(0));
    EXPECT_CALL(mockNvs, nvs_open(StrEq("config"), NVS_READWRITE, _))
        .WillOnce(DoAll(SetArgPointee<2>(nvsHandle), Return(ESP_OK)));
    EXPECT_CALL(mockNvs, nvs_set_str(nvsHandle, StrEq("wifi_ssid"), StrEq("StageWifi")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockNvs, nvs_set_str(nvsHandle, StrEq("device_name"), StrEq("master_1")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockNvs, nvs_commit(nvsHandle)).WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockNvs, nvs_close(nvsHandle)).Times(1);
    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer, httpd_resp_send(&request, _, _))
        .WillOnce([&](httpd_req_t *, const char *bodyOut, size_t len) {
            response.assign(bodyOut, len);
            return ESP_OK;
        });

    EXPECT_EQ(ESP_OK, apiConfig.put_config_handler(&request));
    EXPECT_THAT(response, HasSubstr("Configuration updated"));
    EXPECT_THAT(response, HasSubstr("StageWifi"));
    EXPECT_THAT(response, HasSubstr("master_1"));
}
