#pragma once
#include <gmock/gmock.h>
#include "IMasterBridge.hpp"

class MockIMasterBridge : public IMasterBridge
{
public:
    MOCK_METHOD(esp_err_t, init, (), (override));
    MOCK_METHOD(esp_err_t, sendMessage, (const uint8_t[6], const std::vector<uint8_t> &), (override));
    MOCK_METHOD(esp_err_t, broadcastMessage, (const std::vector<uint8_t> &), (override));
    MOCK_METHOD(esp_err_t, addSlave, (const esp_now_peer_info_t &), (override));
};
