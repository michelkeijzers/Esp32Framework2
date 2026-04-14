#pragma once

#ifdef ESP_PLATFORM
#include "esp_err.h"
#else

#include <cstdio>

inline void esp_log_error(const char* tag, const char* msg) {
    std::fprintf(stderr, "[ERROR][%s] %s\n", tag, msg);
}

// Stand-in types for non-ESP32 platforms
using esp_err_t = int;

inline constexpr esp_err_t kEspOk = 0;
inline constexpr esp_err_t kEspFail = -1;
inline constexpr esp_err_t kEspErrNoMem = 0x101;
inline constexpr esp_err_t kEspErrInvalidArg = 0x102;
inline constexpr esp_err_t kEspErrInvalidState = 0x103;
inline constexpr esp_err_t kEspErrInvalidSize = 0x104;
inline constexpr esp_err_t kEspErrNotFound = 0x105;
inline constexpr esp_err_t kEspErrNotSupported = 0x106;
inline constexpr esp_err_t kEspErrTimeout = 0x107;
inline constexpr esp_err_t kEspErrInvalidResponse = 0x108;
inline constexpr esp_err_t kEspErrInvalidCrc = 0x109;
inline constexpr esp_err_t kEspErrInvalidVersion = 0x10A;
inline constexpr esp_err_t kEspErrInvalidMac = 0x10B;
inline constexpr esp_err_t kEspErrNotFinished = 0x10C;
inline constexpr esp_err_t kEspErrNotAllowed = 0x10D;
inline constexpr esp_err_t kEspErrWifiBase = 0x3000;
inline constexpr esp_err_t kEspErrMeshBase = 0x4000;
inline constexpr esp_err_t kEspErrFlashBase = 0x6000;
inline constexpr esp_err_t kEspErrHwCryptoBase = 0xc000;
inline constexpr esp_err_t kEspErrMemprotBase = 0xd000;

using esp_err_to_namer_t = const char*;

inline esp_err_to_namer_t esp_err_to_name(esp_err_t code) {
    switch (code) {
        case kEspOk:
            return "ESP_OK";
        case kEspFail:
            return "ESP_FAIL";
        case kEspErrNoMem:
            return "ESP_ERR_NO_MEM";
        case kEspErrInvalidArg:
            return "ESP_ERR_INVALID_ARG";
        case kEspErrInvalidState:
            return "ESP_ERR_INVALID_STATE";
        case kEspErrInvalidSize:
            return "ESP_ERR_INVALID_SIZE";
        case kEspErrNotFound:
            return "ESP_ERR_NOT_FOUND";
        case kEspErrNotSupported:
            return "ESP_ERR_NOT_SUPPORTED";
        case kEspErrTimeout:
            return "ESP_ERR_TIMEOUT";
        case kEspErrInvalidResponse:
            return "ESP_ERR_INVALID_RESPONSE";
        case kEspErrInvalidCrc:
            return "ESP_ERR_INVALID_CRC";
        case kEspErrInvalidVersion:
            return "ESP_ERR_INVALID_VERSION";
        case kEspErrInvalidMac:
            return "ESP_ERR_INVALID_MAC";
        case kEspErrNotFinished:
            return "ESP_ERR_NOT_FINISHED";
        case kEspErrNotAllowed:
            return "ESP_ERR_NOT_ALLOWED";
        default:
            return "UNKNOWN_ERROR_CODE";
    }
}

#define ESP_OK (kEspOk)     /*!< esp_err_t value indicating success (no error) */
#define ESP_FAIL (kEspFail) /*!< Generic esp_err_t code indicating failure */

#define ESP_ERR_NO_MEM (kEspErrNoMem)                     /*!< Out of memory */
#define ESP_ERR_INVALID_ARG (kEspErrInvalidArg)           /*!< Invalid argument */
#define ESP_ERR_INVALID_STATE (kEspErrInvalidState)       /*!< Invalid state */
#define ESP_ERR_INVALID_SIZE (kEspErrInvalidSize)         /*!< Invalid size */
#define ESP_ERR_NOT_FOUND (kEspErrNotFound)               /*!< Requested resource not found */
#define ESP_ERR_NOT_SUPPORTED (kEspErrNotSupported)       /*!< Operation or feature not supported */
#define ESP_ERR_TIMEOUT (kEspErrTimeout)                  /*!< Operation timed out */
#define ESP_ERR_INVALID_RESPONSE (kEspErrInvalidResponse) /*!< Received response was invalid */
#define ESP_ERR_INVALID_CRC (kEspErrInvalidCrc)           /*!< CRC or checksum was invalid */
#define ESP_ERR_INVALID_VERSION (kEspErrInvalidVersion)   /*!< Version was invalid */
#define ESP_ERR_INVALID_MAC (kEspErrInvalidMac)           /*!< MAC address was invalid */
#define ESP_ERR_NOT_FINISHED (kEspErrNotFinished)         /*!< Operation has not fully completed */
#define ESP_ERR_NOT_ALLOWED (kEspErrNotAllowed)           /*!< Operation is not allowed */

#define ESP_ERR_WIFI_BASE (kEspErrWifiBase)   /*!< Starting number of WiFi error codes */
#define ESP_ERR_MESH_BASE (kEspErrMeshBase)   /*!< Starting number of MESH error codes */
#define ESP_ERR_FLASH_BASE (kEspErrFlashBase) /*!< Starting number of flash error codes */
#define ESP_ERR_HW_CRYPTO_BASE \
    (kEspErrHwCryptoBase) /*!< Starting number of HW cryptography module error codes */
#define ESP_ERR_MEMPROT_BASE \
    (kEspErrMemprotBase) /*!< Starting number of Memory Protection API error codes */

#define portMAX_DELAY (TickType_t)0xffffffffUL

#endif
