#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "MockIEspHttpServer.hpp"
#include "MockIPresetManager.hpp"
#include "project_dmx_controller/web_server_task/apis/ApiPresets.hpp"

using ::testing::_;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SaveArg;
using ::testing::StrEq;

class ApiPresetsTest : public ::testing::Test {
   protected:
    MockIEspHttpServer mockEspHttpServer;
    MockIPresetManager mockPresetManager;
    ApiPresets apiPresets{mockEspHttpServer, mockPresetManager};
    httpd_req_t request{};
};

TEST_F(ApiPresetsTest, GetPresetsHandlerSerializesPresetList) {
    std::string response;
    Preset presetA(1, "SceneA", true);
    Preset presetB(2, "SceneB", false);

    EXPECT_CALL(mockPresetManager, get_presets())
        .WillOnce(Return(std::vector<Preset>{presetA, presetB}));
    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer, httpd_resp_send(&request, _, HTTPD_RESP_USE_STRLEN))
        .WillOnce([&](httpd_req_t*, const char* body, size_t) {
            response = body;
            return ESP_OK;
        });

    EXPECT_EQ(ESP_OK, apiPresets.get_presets_handler(&request));
    EXPECT_EQ(
        "[{\"number\":1,\"name\":\"SceneA\",\"active\":true},{\"number\":2,\"name\":\"SceneB\","
        "\"active\":false}]",
        response);
}

TEST_F(ApiPresetsTest, SavePresetHandlerReturnsCommitFailure) {
    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockPresetManager, commit()).WillOnce(Return(ESP_FAIL));
    EXPECT_CALL(mockEspHttpServer,
                httpd_resp_send(&request, StrEq("{\"ack\":\"nok\"}"), HTTPD_RESP_USE_STRLEN))
        .WillOnce(Return(ESP_OK));

    EXPECT_EQ(ESP_FAIL, apiPresets.save_preset_handler(&request));
}

TEST_F(ApiPresetsTest, SavePresetHandlerReturnsAckOkWhenCommitSucceeds) {
    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockPresetManager, commit()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer,
                httpd_resp_send(&request, StrEq("{\"ack\":\"ok\"}"), HTTPD_RESP_USE_STRLEN))
        .WillOnce(Return(ESP_OK));

    EXPECT_EQ(ESP_OK, apiPresets.save_preset_handler(&request));
}

TEST_F(ApiPresetsTest, GetActivePresetNumbersHandlerReturnsNumberArray) {
    std::string response;

    EXPECT_CALL(mockPresetManager, get_active_presets()).WillOnce(Return(std::vector<int>{2, 5}));
    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer, httpd_resp_send(&request, _, HTTPD_RESP_USE_STRLEN))
        .WillOnce([&](httpd_req_t*, const char* body, size_t) {
            response = body;
            return ESP_OK;
        });

    EXPECT_EQ(ESP_OK, apiPresets.get_active_preset_numbers_handler(&request));
    EXPECT_EQ("[2,5]", response);
}

TEST_F(ApiPresetsTest, SelectPresetHandlerReturnsPresetNameOnSuccess) {
    std::string response;
    Preset preset(7, "FrontWash", true);
    request.uri = "/api/v1/select_preset/7";

    EXPECT_CALL(mockPresetManager, select_preset(7)).WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockPresetManager, commit()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockPresetManager, get_preset(7)).WillOnce(Return(&preset));
    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer, httpd_resp_send(&request, _, HTTPD_RESP_USE_STRLEN))
        .WillOnce([&](httpd_req_t*, const char* body, size_t) {
            response = body;
            return ESP_OK;
        });

    EXPECT_EQ(ESP_OK, apiPresets.select_preset_handler(&request));
    EXPECT_EQ("{\"preset_name\":\"FrontWash\"}", response);
}

TEST_F(ApiPresetsTest, SelectPresetHandlerReturnsNokWhenSelectFails) {
    request.uri = "/api/v1/select_preset/7";

    EXPECT_CALL(mockPresetManager, select_preset(7)).WillOnce(Return(ESP_FAIL));
    EXPECT_CALL(mockPresetManager, commit()).Times(0);
    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer,
                httpd_resp_send(&request, StrEq("{\"ack\":\"nok\"}"), HTTPD_RESP_USE_STRLEN))
        .WillOnce(Return(ESP_OK));

    EXPECT_EQ(ESP_FAIL, apiPresets.select_preset_handler(&request));
}

TEST_F(ApiPresetsTest, SelectPresetHandlerReturnsNokWhenCommitFails) {
    request.uri = "/api/v1/select_preset/7";

    EXPECT_CALL(mockPresetManager, select_preset(7)).WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockPresetManager, commit()).WillOnce(Return(ESP_FAIL));
    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer,
                httpd_resp_send(&request, StrEq("{\"ack\":\"nok\"}"), HTTPD_RESP_USE_STRLEN))
        .WillOnce(Return(ESP_OK));

    EXPECT_EQ(ESP_FAIL, apiPresets.select_preset_handler(&request));
}

TEST_F(ApiPresetsTest, BlackoutHandlerReturnsBlackoutPresetName) {
    EXPECT_CALL(mockPresetManager, blackout()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockPresetManager, commit()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(
        mockEspHttpServer,
        httpd_resp_send(&request, StrEq("{\"preset_name\":\"Blackout\"}"), HTTPD_RESP_USE_STRLEN))
        .WillOnce(Return(ESP_OK));

    EXPECT_EQ(ESP_OK, apiPresets.blackout_handler(&request));
}

TEST_F(ApiPresetsTest, InsertPresetAtHandlerCreatesNewPresetAndReturnsUpdatedList) {
    std::string response;
    Preset savedPreset;
    request.uri = "/api/v1/presets/4/insert_at";

    EXPECT_CALL(mockPresetManager, save_preset(_))
        .WillOnce(DoAll(SaveArg<0>(&savedPreset), Return(ESP_OK)));
    EXPECT_CALL(mockPresetManager, commit()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockPresetManager, get_presets()).WillOnce([&]() {
        return std::vector<Preset>{savedPreset};
    });
    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer, httpd_resp_send(&request, _, HTTPD_RESP_USE_STRLEN))
        .WillOnce([&](httpd_req_t*, const char* body, size_t) {
            response = body;
            return ESP_OK;
        });

    EXPECT_EQ(ESP_OK, apiPresets.insert_preset_at_handler(&request));
    EXPECT_EQ(4, savedPreset.number);
    EXPECT_EQ("New Preset", savedPreset.name);
    EXPECT_EQ("[{\"number\":4,\"name\":\"New Preset\",\"active\":false}]", response);
}

TEST_F(ApiPresetsTest, MovePresetUpHandlerReturnsUpdatedPresetList) {
    std::string response;
    request.uri = "/api/v1/presets/6/move_up";
    Preset preset(6, "Moved", false);

    EXPECT_CALL(mockPresetManager, move_preset(6, true)).WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockPresetManager, commit()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockPresetManager, get_presets()).WillOnce(Return(std::vector<Preset>{preset}));
    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer, httpd_resp_send(&request, _, HTTPD_RESP_USE_STRLEN))
        .WillOnce([&](httpd_req_t*, const char* body, size_t) {
            response = body;
            return ESP_OK;
        });

    EXPECT_EQ(ESP_OK, apiPresets.move_preset_up_handler(&request));
    EXPECT_EQ("[{\"number\":6,\"name\":\"Moved\",\"active\":false}]", response);
}

TEST_F(ApiPresetsTest, DeletePresetHandlerReturnsNokWhenDeleteFails) {
    request.uri = "/api/v1/presets/4";

    EXPECT_CALL(mockPresetManager, delete_preset(4)).WillOnce(Return(ESP_FAIL));
    EXPECT_CALL(mockPresetManager, commit()).Times(0);
    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer,
                httpd_resp_send(&request, StrEq("{\"ack\":\"nok\"}"), HTTPD_RESP_USE_STRLEN))
        .WillOnce(Return(ESP_OK));

    EXPECT_EQ(ESP_FAIL, apiPresets.delete_preset_handler(&request));
}

TEST_F(ApiPresetsTest, SwapActivationHandlerReturnsUpdatedPresetList) {
    std::string response;
    request.uri = "/api/v1/presets/9/swap_activation";
    Preset preset(9, "Live", true);

    EXPECT_CALL(mockPresetManager, swap_activation(9)).WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockPresetManager, commit()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockPresetManager, get_presets()).WillOnce(Return(std::vector<Preset>{preset}));
    EXPECT_CALL(mockEspHttpServer, httpd_resp_set_type(&request, StrEq("application/json")))
        .WillOnce(Return(ESP_OK));
    EXPECT_CALL(mockEspHttpServer, httpd_resp_send(&request, _, HTTPD_RESP_USE_STRLEN))
        .WillOnce([&](httpd_req_t*, const char* body, size_t) {
            response = body;
            return ESP_OK;
        });

    EXPECT_EQ(ESP_OK, apiPresets.swap_preset_activation_handler(&request));
    EXPECT_EQ("[{\"number\":9,\"name\":\"Live\",\"active\":true}]", response);
}
