#include "common/esp/esp_file_systems/EspLittleFs.hpp"
#include "LowLevelStubCounters.hpp"

esp_err_t EspLittleFs::esp_littlefs_mount(const esp_vfs_littlefs_conf_t *)
{
    ++low_level_stubs::counters().littlefs_mount_calls;
    return ESP_OK;
}

esp_err_t EspLittleFs::esp_littlefs_format(const char *)
{
    return ESP_OK;
}
