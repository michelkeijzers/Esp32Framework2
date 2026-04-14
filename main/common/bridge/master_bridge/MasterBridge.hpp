#pragma once

#include <cstdint>
#include <span>

#include "../../esp/esp_now/IEspNow.hpp"
#include "IMasterBridge.hpp"

/**
 * Concrete master-side bridge in the Remote Proxy pattern.
 *
 * Owns an IEspNow reference and uses it to forward messages to slave nodes
 * over the ESP-NOW wireless protocol.
 */
class MasterBridge : public IMasterBridge {
   public:
    explicit MasterBridge(IEspNow &espNow);
    ~MasterBridge() override = default;

    [[nodiscard]] esp_err_t init() override;

    [[nodiscard]] esp_err_t sendMessage(std::span<const uint8_t, kEspNowMacAddressLength> targetMac,
                                        std::span<const uint8_t> data) override;

    [[nodiscard]] esp_err_t broadcastMessage(std::span<const uint8_t> data) override;

    [[nodiscard]] esp_err_t addSlave(const esp_now_peer_info_t &peerInfo) override;

   private:
    IEspNow &espNow_;
};
