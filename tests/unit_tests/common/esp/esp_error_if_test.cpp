#include "common/esp/esp_error/esp_error_if.hpp"

#include <gtest/gtest.h>

TEST(EspErrorIfTest, MapsKnownErrorCodesToNames) {
    EXPECT_STREQ("ESP_OK", esp_err_to_name(ESP_OK));
    EXPECT_STREQ("ESP_FAIL", esp_err_to_name(ESP_FAIL));
    EXPECT_STREQ("ESP_ERR_NO_MEM", esp_err_to_name(ESP_ERR_NO_MEM));
    EXPECT_STREQ("ESP_ERR_INVALID_ARG", esp_err_to_name(ESP_ERR_INVALID_ARG));
    EXPECT_STREQ("ESP_ERR_INVALID_STATE", esp_err_to_name(ESP_ERR_INVALID_STATE));
    EXPECT_STREQ("ESP_ERR_INVALID_SIZE", esp_err_to_name(ESP_ERR_INVALID_SIZE));
    EXPECT_STREQ("ESP_ERR_NOT_FOUND", esp_err_to_name(ESP_ERR_NOT_FOUND));
    EXPECT_STREQ("ESP_ERR_NOT_SUPPORTED", esp_err_to_name(ESP_ERR_NOT_SUPPORTED));
    EXPECT_STREQ("ESP_ERR_TIMEOUT", esp_err_to_name(ESP_ERR_TIMEOUT));
    EXPECT_STREQ("ESP_ERR_INVALID_RESPONSE", esp_err_to_name(ESP_ERR_INVALID_RESPONSE));
    EXPECT_STREQ("ESP_ERR_INVALID_CRC", esp_err_to_name(ESP_ERR_INVALID_CRC));
    EXPECT_STREQ("ESP_ERR_INVALID_VERSION", esp_err_to_name(ESP_ERR_INVALID_VERSION));
    EXPECT_STREQ("ESP_ERR_INVALID_MAC", esp_err_to_name(ESP_ERR_INVALID_MAC));
    EXPECT_STREQ("ESP_ERR_NOT_FINISHED", esp_err_to_name(ESP_ERR_NOT_FINISHED));
    EXPECT_STREQ("ESP_ERR_NOT_ALLOWED", esp_err_to_name(ESP_ERR_NOT_ALLOWED));
}

TEST(EspErrorIfTest, ReturnsUnknownForUnmappedCodes) {
    EXPECT_STREQ("UNKNOWN_ERROR_CODE", esp_err_to_name(0x7FFFFFFF));
    EXPECT_STREQ("UNKNOWN_ERROR_CODE", esp_err_to_name(ESP_ERR_WIFI_BASE));
}
