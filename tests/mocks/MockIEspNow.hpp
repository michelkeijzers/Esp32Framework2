#pragma once
#include <gmock/gmock.h>
#include "IEspNow.hpp"

class MockIEspNow : public IEspNow
{
public:
    MOCK_METHOD(esp_err_t, init, (), (override));
    MOCK_METHOD(esp_err_t, deinit, (), (override));
    MOCK_METHOD(esp_err_t, connectPeer, (const esp_now_peer_info_t *), (override));
    MOCK_METHOD(esp_err_t, disconnectPeer, (const uint8_t *), (override));
    MOCK_METHOD(esp_err_t, sendMessage, (const uint8_t *, size_t), (override));
};
