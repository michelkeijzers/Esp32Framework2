#pragma once

#include "ISlaveBridge.hpp"
#include "../../esp/esp_now/IEspNow.hpp"

#include <cstdint>
#include <vector>
#include <functional>

/**
 * Concrete slave-side bridge in the Remote Proxy pattern.
 *
 * Registers an ESP-NOW receive callback and forwards decoded messages to
 * registered application-level handlers on the slave node.
 */
class SlaveBridge : public ISlaveBridge
{
public:
    explicit SlaveBridge(IEspNow &espNow);
    ~SlaveBridge() override = default;

    esp_err_t init() override;

    esp_err_t registerMaster(const esp_now_peer_info_t &masterPeerInfo) override;

    void setReceiveCallback(
        std::function<void(const std::vector<uint8_t> &)> callback) override;

private:
    IEspNow   &espNow_;
    std::function<void(const std::vector<uint8_t> &)> receiveCallback_;
};
