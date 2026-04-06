#pragma once
#include <gmock/gmock.h>
#include "ISlaveBridge.hpp"

#include <span>

class MockISlaveBridge : public ISlaveBridge
{
public:
    MOCK_METHOD(esp_err_t, init, (), (override));
    MOCK_METHOD(esp_err_t, registerMaster, (const esp_now_peer_info_t &), (override));
    MOCK_METHOD(void, setReceiveCallback,
                (std::function<void(std::span<const uint8_t>)>), (override));
};
