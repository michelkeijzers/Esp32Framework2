#pragma once

#include "IMasterBridge.hpp"
#include "../../esp/esp_now/IEspNow.hpp"

#include <cstdint>
#include <vector>

/**
 * Concrete master-side bridge in the Remote Proxy pattern.
 *
 * Owns an IEspNow reference and uses it to forward messages to slave nodes
 * over the ESP-NOW wireless protocol.
 */
class MasterBridge : public IMasterBridge
{
public:
    explicit MasterBridge(IEspNow &espNow);
    ~MasterBridge() override = default;

    esp_err_t init() override;

    esp_err_t sendMessage(const uint8_t targetMac[6],
                          const std::vector<uint8_t> &data) override;

    esp_err_t broadcastMessage(const std::vector<uint8_t> &data) override;

    esp_err_t addSlave(const esp_now_peer_info_t &peerInfo) override;

private:
    IEspNow &espNow_;
};
