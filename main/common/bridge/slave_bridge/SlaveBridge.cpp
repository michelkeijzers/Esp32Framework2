#include "SlaveBridge.hpp"

SlaveBridge::SlaveBridge(IEspNow &espNow)
    : espNow_(espNow)
{
}

esp_err_t SlaveBridge::init()
{
    esp_err_t ret = espNow_.init();
    if (ret != ESP_OK)
    {
        return ret;
    }

    // TODO: register ESP-NOW receive callback on espNow_ and decode incoming
    // messages, then invoke receiveCallback_ if set.

    return ESP_OK;
}

esp_err_t SlaveBridge::registerMaster(const esp_now_peer_info_t &masterPeerInfo)
{
    return espNow_.connectPeer(&masterPeerInfo);
}

void SlaveBridge::setReceiveCallback(
    std::function<void(const std::vector<uint8_t> &)> callback)
{
    receiveCallback_ = std::move(callback);
}
