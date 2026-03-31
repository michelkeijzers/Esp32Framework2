#pragma once

#ifdef ESP_PLATFORM
#include "esp_err.h"
#else

#include <cstdio>
inline void esp_log_error(const char* tag, const char* msg) {
    std::fprintf(stderr, "[ERROR][%s] %s\n", tag, msg);
}


// Stand-in types for non-ESP32 platforms
typedef int esp_err_t;

typedef const char* esp_err_to_namer_t;
inline esp_err_to_namer_t esp_err_to_name(esp_err_t code)
{
    switch (code)
    {
        case 0: return "ESP_OK";
        case -1: return "ESP_FAIL";
        case 0x101: return "ESP_ERR_NO_MEM";
        case 0x102: return "ESP_ERR_INVALID_ARG";
        case 0x103: return "ESP_ERR_INVALID_STATE";
        case 0x104: return "ESP_ERR_INVALID_SIZE";
        case 0x105: return "ESP_ERR_NOT_FOUND";
        case 0x106: return "ESP_ERR_NOT_SUPPORTED";
        case 0x107: return "ESP_ERR_TIMEOUT";
        case 0x108: return "ESP_ERR_INVALID_RESPONSE";
        case 0x109: return "ESP_ERR_INVALID_CRC";
        case 0x10A: return "ESP_ERR_INVALID_VERSION";
        case 0x10B: return "ESP_ERR_INVALID_MAC";
        case 0x10C: return "ESP_ERR_NOT_FINISHED";
        case 0x10D: return "ESP_ERR_NOT_ALLOWED";
        default: return "UNKNOWN_ERROR_CODE";
    }
}

#define ESP_OK          0       /*!< esp_err_t value indicating success (no error) */
#define ESP_FAIL        -1      /*!< Generic esp_err_t code indicating failure */

#define ESP_ERR_NO_MEM              0x101   /*!< Out of memory */
#define ESP_ERR_INVALID_ARG         0x102   /*!< Invalid argument */
#define ESP_ERR_INVALID_STATE       0x103   /*!< Invalid state */
#define ESP_ERR_INVALID_SIZE        0x104   /*!< Invalid size */
#define ESP_ERR_NOT_FOUND           0x105   /*!< Requested resource not found */
#define ESP_ERR_NOT_SUPPORTED       0x106   /*!< Operation or feature not supported */
#define ESP_ERR_TIMEOUT             0x107   /*!< Operation timed out */
#define ESP_ERR_INVALID_RESPONSE    0x108   /*!< Received response was invalid */
#define ESP_ERR_INVALID_CRC         0x109   /*!< CRC or checksum was invalid */
#define ESP_ERR_INVALID_VERSION     0x10A   /*!< Version was invalid */
#define ESP_ERR_INVALID_MAC         0x10B   /*!< MAC address was invalid */
#define ESP_ERR_NOT_FINISHED        0x10C   /*!< Operation has not fully completed */
#define ESP_ERR_NOT_ALLOWED         0x10D   /*!< Operation is not allowed */

#define ESP_ERR_WIFI_BASE           0x3000  /*!< Starting number of WiFi error codes */
#define ESP_ERR_MESH_BASE           0x4000  /*!< Starting number of MESH error codes */
#define ESP_ERR_FLASH_BASE          0x6000  /*!< Starting number of flash error codes */
#define ESP_ERR_HW_CRYPTO_BASE      0xc000  /*!< Starting number of HW cryptography module error codes */
#define ESP_ERR_MEMPROT_BASE        0xd000  /*!< Starting number of Memory Protection API error codes */


#endif
