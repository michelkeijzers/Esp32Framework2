#include "MasterBridge.hpp"

MasterBridge::MasterBridge(IEspNow &espNow)
    : espNow_(espNow)
{
}

esp_err_t MasterBridge::init()
{
    return espNow_.init();
}

esp_err_t MasterBridge::sendMessage(const uint8_t targetMac[6],
                                    const std::vector<uint8_t> &data)
{
    // TODO: implement message serialisation / protocol framing before sending.
    // Note: IEspNow::sendMessage does not currently accept a target MAC;
    // extend IEspNow or maintain a MAC->peer mapping here if unicast is needed.
    (void)targetMac;
    return espNow_.sendMessage(data.data(), data.size());
}

esp_err_t MasterBridge::broadcastMessage(const std::vector<uint8_t> &data)
{
    return espNow_.sendMessage(data.data(), data.size());
}

esp_err_t MasterBridge::addSlave(const esp_now_peer_info_t &peerInfo)
{
    return espNow_.connectPeer(&peerInfo);
}
