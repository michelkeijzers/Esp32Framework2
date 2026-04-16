#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "MockIEspHttpServer.hpp"
#include "MockIPresetManager.hpp"
#include "project_dmx_controller/web_server_task/apis/ApiPresetValues.hpp"

using ::testing::_;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SaveArg;
using ::testing::StrEq;

class ApiPresetValuesTest : public ::testing::Test {
   protected:
    MockIEspHttpServer mockEspHttpServer;
    MockIPresetManager mockPresetManager;
    ApiPresetValues apiPresetValues{mockEspHttpServer, mockPresetManager};
    httpd_req_t request{};
};

TEST_F(ApiPresetValuesTest, GetPresetValuesHandlerReturnsNotFoundWhenPresetMissing) {
    request.uri = "/api/v1/preset_values/9";

    EXPECT_CALL(mockPresetManager, get_preset(9)).WillOnce(Return(nullptr));
    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(
        mockEspHttpServer,
        httpd_resp_send(&request, StrEq("{\"error\":\"preset not found\"}"), HTTPD_RESP_USE_STRLEN))
        .WillOnce(Return(ESP_OK));

    EXPECT_EQ(ESP_ERR_NOT_FOUND, apiPresetValues.get_preset_values_handler(&request));
}

TEST_F(ApiPresetValuesTest, GetPresetValuesHandlerReturnsSerializedValues) {
    std::string response;
    Preset preset(3, "Preset3", false);
    preset.dmx_values[0] = 10;
    preset.dmx_values[1] = 20;
    preset.dmx_values[2] = 30;
    preset.dmx_values.resize(3);
    request.uri = "/api/v1/preset_values/3";

    EXPECT_CALL(mockPresetManager, get_preset(3)).WillOnce(Return(&preset));
    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer, httpd_resp_send(&request, _, HTTPD_RESP_USE_STRLEN))
        .WillOnce([&](httpd_req_t *, const char *body, size_t) {
            response = body;
            return ESP_OK;
        });

    EXPECT_EQ(ESP_OK, apiPresetValues.get_preset_values_handler(&request));
    EXPECT_EQ("[10,20,30]", response);
}

TEST_F(ApiPresetValuesTest, SetPresetValueHandlerReturnsNotFoundWhenPresetMissing) {
    request.uri = "/api/v1/preset_value/2/5/200";

    EXPECT_CALL(mockPresetManager, get_preset(2)).WillOnce(Return(nullptr));
    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(
        mockEspHttpServer,
        httpd_resp_send(&request, StrEq("{\"error\":\"preset not found\"}"), HTTPD_RESP_USE_STRLEN))
        .WillOnce(Return(ESP_OK));

    EXPECT_EQ(ESP_ERR_NOT_FOUND, apiPresetValues.set_preset_value_handler(&request));
}

TEST_F(ApiPresetValuesTest, SetPresetValueHandlerRejectsOutOfRangeChannel) {
    Preset preset(2, "Preset2", false);
    request.uri = "/api/v1/preset_value/2/700/255";

    EXPECT_CALL(mockPresetManager, get_preset(2)).WillOnce(Return(&preset));
    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer,
                httpd_resp_send(&request, StrEq("{\"error\":\"channel index out of range\"}"),
                                HTTPD_RESP_USE_STRLEN))
        .WillOnce(Return(ESP_OK));

    EXPECT_EQ(ESP_ERR_INVALID_ARG, apiPresetValues.set_preset_value_handler(&request));
}

TEST_F(ApiPresetValuesTest, SetPresetValueHandlerReturnsInvalidArgForMissingParameters) {
    request.uri = "/api/v1/preset_value";

    EXPECT_CALL(mockPresetManager, get_preset(_)).Times(0);
    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer,
                httpd_resp_send(&request, StrEq("{\"error\":\"missing parameters\"}"),
                                HTTPD_RESP_USE_STRLEN))
        .WillOnce(Return(ESP_OK));

    EXPECT_EQ(ESP_ERR_INVALID_ARG, apiPresetValues.set_preset_value_handler(&request));
}

TEST_F(ApiPresetValuesTest, SetPresetValueHandlerReturnsSaveError) {
    Preset preset(5, "Preset5", false);
    request.uri = "/api/v1/preset_value/5/12/99";

    EXPECT_CALL(mockPresetManager, get_preset(5)).WillOnce(Return(&preset));
    EXPECT_CALL(mockPresetManager, save_preset(_)).WillOnce(Return(ESP_FAIL));
    EXPECT_CALL(mockPresetManager, commit()).Times(0);
    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer,
                httpd_resp_send(&request, StrEq("{\"error\":\"failed to save preset\"}"),
                                HTTPD_RESP_USE_STRLEN))
        .WillOnce(Return(ESP_OK));

    EXPECT_EQ(ESP_FAIL, apiPresetValues.set_preset_value_handler(&request));
}

TEST_F(ApiPresetValuesTest, SetPresetValueHandlerReturnsCommitError) {
    Preset preset(5, "Preset5", false);
    request.uri = "/api/v1/preset_value/5/12/99";

    EXPECT_CALL(mockPresetManager, get_preset(5)).WillOnce(Return(&preset));
    EXPECT_CALL(mockPresetManager, save_preset(_)).WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockPresetManager, commit()).WillOnce(Return(ESP_FAIL));
    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(
        mockEspHttpServer,
        httpd_resp_send(&request, StrEq("{\"error\":\"failed to commit\"}"), HTTPD_RESP_USE_STRLEN))
        .WillOnce(Return(ESP_OK));

    EXPECT_EQ(ESP_FAIL, apiPresetValues.set_preset_value_handler(&request));
}

TEST_F(ApiPresetValuesTest, SetPresetValueHandlerDefaultsValueToZeroForTwoParameterUri) {
    std::string response;
    Preset preset(5, "Preset5", false);
    Preset savedPreset;
    request.uri = "/api/v1/preset_value/5/12";

    EXPECT_CALL(mockPresetManager, get_preset(5)).WillOnce(Return(&preset));
    EXPECT_CALL(mockPresetManager, save_preset(_))
        .WillOnce(DoAll(SaveArg<0>(&savedPreset), Return(ESP_OK)));
    EXPECT_CALL(mockPresetManager, commit()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer, httpd_resp_send(&request, _, HTTPD_RESP_USE_STRLEN))
        .WillOnce([&](httpd_req_t*, const char* body, size_t) {
            response = body;
            return ESP_OK;
        });

    EXPECT_EQ(ESP_OK, apiPresetValues.set_preset_value_handler(&request));
    EXPECT_EQ(0, savedPreset.dmx_values[12]);
    EXPECT_EQ("{\"index\":12,\"value\":0}", response);
}

TEST_F(ApiPresetValuesTest, SetPresetValueHandlerUpdatesPresetAndCommits) {
    std::string response;
    Preset preset(5, "Preset5", false);
    Preset savedPreset;
    request.uri = "/api/v1/preset_value/5/12/99";

    EXPECT_CALL(mockPresetManager, get_preset(5)).WillOnce(Return(&preset));
    EXPECT_CALL(mockPresetManager, save_preset(_))
        .WillOnce(DoAll(SaveArg<0>(&savedPreset), Return(ESP_OK)));
    EXPECT_CALL(mockPresetManager, commit()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer, httpd_resp_send(&request, _, HTTPD_RESP_USE_STRLEN))
        .WillOnce([&](httpd_req_t *, const char *body, size_t) {
            response = body;
            return ESP_OK;
        });

    EXPECT_EQ(ESP_OK, apiPresetValues.set_preset_value_handler(&request));
    EXPECT_EQ(99, savedPreset.dmx_values[12]);
    EXPECT_EQ("{\"index\":12,\"value\":99}", response);
}
