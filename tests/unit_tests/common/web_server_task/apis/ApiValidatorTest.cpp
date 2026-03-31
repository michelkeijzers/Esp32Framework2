#include <gtest/gtest.h>
#include "ApiValidator.hpp"

// ─────────────────────────────────────────────────────────────
// is_valid_ssid
// ─────────────────────────────────────────────────────────────

TEST(ApiValidatorTest, SsidNullReturnsFalse)       { EXPECT_FALSE(ApiValidator::is_valid_ssid(nullptr)); }
TEST(ApiValidatorTest, SsidEmptyReturnsFalse)      { EXPECT_FALSE(ApiValidator::is_valid_ssid("")); }
TEST(ApiValidatorTest, SsidOneCharReturnsTrue)     { EXPECT_TRUE(ApiValidator::is_valid_ssid("A")); }
TEST(ApiValidatorTest, Ssid32CharsReturnsTrue)     { EXPECT_TRUE(ApiValidator::is_valid_ssid("12345678901234567890123456789012")); }
TEST(ApiValidatorTest, Ssid33CharsReturnsFalse)    { EXPECT_FALSE(ApiValidator::is_valid_ssid("123456789012345678901234567890123")); }

// ─────────────────────────────────────────────────────────────
// is_valid_wifi_password
// ─────────────────────────────────────────────────────────────

TEST(ApiValidatorTest, WifiPassNullReturnsFalse)   { EXPECT_FALSE(ApiValidator::is_valid_wifi_password(nullptr)); }
TEST(ApiValidatorTest, WifiPass7ReturnsFalse)      { EXPECT_FALSE(ApiValidator::is_valid_wifi_password("1234567")); }
TEST(ApiValidatorTest, WifiPass8ReturnsTrue)       { EXPECT_TRUE(ApiValidator::is_valid_wifi_password("12345678")); }
TEST(ApiValidatorTest, WifiPass63ReturnsTrue)
{
    std::string s(63, 'a');
    EXPECT_TRUE(ApiValidator::is_valid_wifi_password(s.c_str()));
}
TEST(ApiValidatorTest, WifiPass64ReturnsFalse)
{
    std::string s(64, 'a');
    EXPECT_FALSE(ApiValidator::is_valid_wifi_password(s.c_str()));
}

// ─────────────────────────────────────────────────────────────
// is_valid_espnow_key
// ─────────────────────────────────────────────────────────────

TEST(ApiValidatorTest, EspNowKeyNullReturnsFalse)  { EXPECT_FALSE(ApiValidator::is_valid_espnow_key(nullptr)); }
TEST(ApiValidatorTest, EspNowKeyShortReturnsFalse) { EXPECT_FALSE(ApiValidator::is_valid_espnow_key("AABBCCDDEEFF00112233445566778")); }
TEST(ApiValidatorTest, EspNowKey32HexReturnsTrue)
{
    EXPECT_TRUE(ApiValidator::is_valid_espnow_key("AABBCCDDEEFF00112233445566778899"));
}
TEST(ApiValidatorTest, EspNowKeyWithNonHexReturnsFalse)
{
    EXPECT_FALSE(ApiValidator::is_valid_espnow_key("ZABBCCDDEEFF00112233445566778899"));
}

// ─────────────────────────────────────────────────────────────
// is_valid_device_name
// ─────────────────────────────────────────────────────────────

TEST(ApiValidatorTest, DeviceNameNullReturnsFalse)  { EXPECT_FALSE(ApiValidator::is_valid_device_name(nullptr)); }
TEST(ApiValidatorTest, DeviceNameEmptyReturnsFalse) { EXPECT_FALSE(ApiValidator::is_valid_device_name("")); }
TEST(ApiValidatorTest, DeviceNameAlphanumReturnsTrue) { EXPECT_TRUE(ApiValidator::is_valid_device_name("master_node1")); }
TEST(ApiValidatorTest, DeviceNameWithSpaceReturnsFalse) { EXPECT_FALSE(ApiValidator::is_valid_device_name("my node")); }
TEST(ApiValidatorTest, DeviceNameWithDashReturnsFalse)  { EXPECT_FALSE(ApiValidator::is_valid_device_name("my-node")); }
TEST(ApiValidatorTest, DeviceName32CharsReturnsTrue)
{
    std::string s(32, 'a');
    EXPECT_TRUE(ApiValidator::is_valid_device_name(s.c_str()));
}
TEST(ApiValidatorTest, DeviceName33CharsReturnsFalse)
{
    std::string s(33, 'a');
    EXPECT_FALSE(ApiValidator::is_valid_device_name(s.c_str()));
}

// ─────────────────────────────────────────────────────────────
// is_valid_json
// ─────────────────────────────────────────────────────────────

TEST(ApiValidatorTest, JsonNullReturnsFalse)  { EXPECT_FALSE(ApiValidator::is_valid_json(nullptr)); }
TEST(ApiValidatorTest, JsonEmptyReturnsFalse) { EXPECT_FALSE(ApiValidator::is_valid_json("")); }
TEST(ApiValidatorTest, JsonNonEmptyReturnsTrue) { EXPECT_TRUE(ApiValidator::is_valid_json("{}")); }

// ─────────────────────────────────────────────────────────────
// is_hex_string
// ─────────────────────────────────────────────────────────────

TEST(ApiValidatorTest, HexStringNullReturnsFalse)     { EXPECT_FALSE(ApiValidator::is_hex_string(nullptr)); }
TEST(ApiValidatorTest, HexStringEmptyReturnsTrue)     { EXPECT_TRUE(ApiValidator::is_hex_string("")); }
TEST(ApiValidatorTest, HexStringLowerCaseReturnsTrue) { EXPECT_TRUE(ApiValidator::is_hex_string("deadbeef")); }
TEST(ApiValidatorTest, HexStringUpperCaseReturnsTrue) { EXPECT_TRUE(ApiValidator::is_hex_string("DEADBEEF")); }
TEST(ApiValidatorTest, HexStringMixedCaseReturnsTrue) { EXPECT_TRUE(ApiValidator::is_hex_string("DeAdBeEf")); }
TEST(ApiValidatorTest, HexStringWithNonHexReturnsFalse) { EXPECT_FALSE(ApiValidator::is_hex_string("ZZZZ")); }
