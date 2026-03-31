#include "common/esp/esp_now/EspNow.hpp"
#include "LowLevelStubCounters.hpp"

EspNow::EspNow() : initialized_(false)
{
}

EspNow::~EspNow() = default;

esp_err_t EspNow::init()
{
    ++low_level_stubs::counters().esp_now_init_calls;
    initialized_ = true;
    return ESP_OK;
}

esp_err_t EspNow::deinit()
{
    initialized_ = false;
    return ESP_OK;
}

esp_err_t EspNow::connectPeer(const esp_now_peer_info_t *)
{
    return ESP_OK;
}

esp_err_t EspNow::disconnectPeer(const uint8_t *)
{
    return ESP_OK;
}

esp_err_t EspNow::sendMessage(const uint8_t *, size_t)
{
    return ESP_OK;
}
