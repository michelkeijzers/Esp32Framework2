#include "ApiValidator.hpp"

bool ApiValidator::is_valid_ssid(const char* ssid) {
    if (!ssid) return false;
    size_t len = strlen(ssid);
    return len >= 1 && len <= 32;
}

bool ApiValidator::is_valid_wifi_password(const char* password) {
    if (!password) return false;
    size_t len = strlen(password);
    return len >= 8 && len <= 63;
}

bool ApiValidator::is_valid_espnow_key(const char* key) {
    if (!key) return false;
    // 16 bytes = 32 hex characters
    if (strlen(key) != 32) return false;
    return is_hex_string(key);
}

bool ApiValidator::is_valid_device_name(const char* name) {
    if (!name) return false;
    size_t len = strlen(name);
    if (len < 1 || len > 32) return false;

    for (size_t i = 0; i < len; i++) {
        char c = name[i];
        if (!isalnum(c) && c != '_') return false;
    }
    return true;
}

bool ApiValidator::is_valid_json(const char* json_str) { return json_str && strlen(json_str) > 0; }

bool ApiValidator::is_hex_string(const char* str) {
    if (!str) return false;
    for (size_t i = 0; i < strlen(str); i++) {
        if (!is_valid_hex_char(str[i])) return false;
    }
    return true;
}

bool ApiValidator::is_valid_hex_char(char c) {
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}
