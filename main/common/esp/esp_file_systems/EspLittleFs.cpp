#include "EspLittleFs.hpp"

#include "esp_littlefs.h"

esp_err_t EspLittleFs::esp_littlefs_mount(const esp_vfs_littlefs_conf_t* conf) {
    return esp_littlefs_mount(conf);
}

esp_err_t EspLittleFs::esp_littlefs_format(const char* partition_label) {
    return esp_littlefs_format(partition_label);
}
