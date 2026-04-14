#pragma once

#include <cstdint>
#include <functional>
#include <span>

#include "../../esp/esp_now/IEspNow.hpp"
#include "ISlaveBridge.hpp"

/**
 * Concrete slave-side bridge in the Remote Proxy pattern.
 *
 * Registers an ESP-NOW receive callback and forwards decoded messages to
 * registered application-level handlers on the slave node.
 */
class SlaveBridge : public ISlaveBridge {
   public:
    explicit SlaveBridge(IEspNow &espNow);
    ~SlaveBridge() override = default;

    [[nodiscard]] esp_err_t init() override;

    [[nodiscard]] esp_err_t registerMaster(const esp_now_peer_info_t &masterPeerInfo) override;

    void setReceiveCallback(std::function<void(std::span<const uint8_t>)> callback) override;

   private:
    IEspNow &espNow_;
    std::function<void(std::span<const uint8_t>)> receiveCallback_;
};
