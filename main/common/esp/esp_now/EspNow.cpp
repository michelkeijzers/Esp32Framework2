#include "EspNow.hpp"
#include "esp_now_if.hpp"

EspNow::EspNow() : initialized_(false)
{
}

EspNow::~EspNow()
{
    deinit();
}

esp_err_t EspNow::init()
{
    return esp_now_init();
}

esp_err_t EspNow::deinit()
{
    return esp_now_deinit();
}

esp_err_t EspNow::connectPeer(const esp_now_peer_info_t* peer_info)
{
    return esp_now_add_peer(peer_info);
}

esp_err_t EspNow::disconnectPeer(const uint8_t *peer_addr)
{

    return esp_now_del_peer(peer_addr);
}

esp_err_t EspNow::sendMessage(const uint8_t *data, size_t len)
{
    return esp_now_send(nullptr, data, len);
}
