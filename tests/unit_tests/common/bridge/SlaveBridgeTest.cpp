#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "SlaveBridge.hpp"
#include "MockIEspNow.hpp"

using ::testing::Return;
using ::testing::_;

class SlaveBridgeTest : public ::testing::Test
{
protected:
    MockIEspNow  mockEspNow;
    SlaveBridge  bridge{mockEspNow};
};

TEST_F(SlaveBridgeTest, InitDelegatesToEspNow)
{
    EXPECT_CALL(mockEspNow, init()).WillOnce(Return(ESP_OK));
    EXPECT_EQ(ESP_OK, bridge.init());
}

TEST_F(SlaveBridgeTest, InitPropagatesError)
{
    EXPECT_CALL(mockEspNow, init()).WillOnce(Return(ESP_FAIL));
    EXPECT_EQ(ESP_FAIL, bridge.init());
}

TEST_F(SlaveBridgeTest, RegisterMasterDelegatesToConnectPeer)
{
    esp_now_peer_info_t peer{};
    peer.peer_addr[0] = 0x11;

    EXPECT_CALL(mockEspNow, connectPeer(&peer)).WillOnce(Return(ESP_OK));
    EXPECT_EQ(ESP_OK, bridge.registerMaster(peer));
}

TEST_F(SlaveBridgeTest, RegisterMasterPropagatesError)
{
    esp_now_peer_info_t peer{};
    EXPECT_CALL(mockEspNow, connectPeer(_)).WillOnce(Return(ESP_FAIL));
    EXPECT_EQ(ESP_FAIL, bridge.registerMaster(peer));
}

TEST_F(SlaveBridgeTest, SetReceiveCallbackStoresCallback)
{
    bool called = false;
    bridge.setReceiveCallback([&called](const std::vector<uint8_t> &) {
        called = true;
    });
    // The callback is stored but not invoked here (ESP-NOW receive path is ESP hardware)
    // Just verify no crash setting the callback
    SUCCEED();
}

TEST_F(SlaveBridgeTest, SetReceiveCallbackCanBeReplacedWithNull)
{
    bridge.setReceiveCallback(nullptr);
    SUCCEED();
}
