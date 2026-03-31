#pragma once
#include <cstring>
#include <cctype>

/**
 * Input Validation Utilities for API Endpoints
 */
class ApiValidator {
public:
    /**
     * Validate Wi-Fi SSID (1-32 characters)
     * @param ssid - SSID string to validate
     * @return true if valid
     */
    static bool is_valid_ssid(const char* ssid);

    /**
     * Validate Wi-Fi Password (8-63 characters)
     * @param password - Password string to validate
     * @return true if valid
     */
    static bool is_valid_wifi_password(const char* password);

    /**
     * Validate ESP-NOW Key (exactly 16 bytes when hex-encoded = 32 characters)
     * @param key - Hex-encoded key string (should be 32 chars of hex digits)
     * @return true if valid
     */
    static bool is_valid_espnow_key(const char* key);

    /**
     * Validate Device Name (1-32 alphanumeric + underscore)
     * @param name - Device name to validate
     * @return true if valid
     */
    static bool is_valid_device_name(const char* name);

    /**
     * Validate JSON string is not null/empty
     * @param json_str - JSON string to validate
     * @return true if valid
     */
    static bool is_valid_json(const char* json_str);

    /**
     * Check if string is valid hexadecimal
     * @param str - String to check
     * @return true if all characters are hex digits
     */
    static bool is_hex_string(const char* str);

private:
    static bool is_valid_hex_char(char c);
};
