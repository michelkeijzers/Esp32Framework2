#pragma once
#include <gmock/gmock.h>
#include "ISlaveBridge.hpp"

class MockISlaveBridge : public ISlaveBridge
{
public:
    MOCK_METHOD(esp_err_t, init, (), (override));
    MOCK_METHOD(esp_err_t, registerMaster, (const esp_now_peer_info_t &), (override));
    MOCK_METHOD(void, setReceiveCallback,
                (std::function<void(const std::vector<uint8_t> &)>), (override));
};
