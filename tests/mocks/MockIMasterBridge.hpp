#pragma once
#include <gmock/gmock.h>
#include "IMasterBridge.hpp"

#include <span>

class MockIMasterBridge : public IMasterBridge
{
public:
    MOCK_METHOD(esp_err_t, init, (), (override));
    MOCK_METHOD(esp_err_t, sendMessage, (const uint8_t[6], std::span<const uint8_t>), (override));
    MOCK_METHOD(esp_err_t, broadcastMessage, (std::span<const uint8_t>), (override));
    MOCK_METHOD(esp_err_t, addSlave, (const esp_now_peer_info_t &), (override));
};
