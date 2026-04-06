#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "MasterBridge.hpp"
#include "MockIEspNow.hpp"

#include <span>
#include <vector>

using ::testing::Return;
using ::testing::_;

// Custom GMock matcher for std::span<const uint8_t> equality.
MATCHER_P(SpanEq, expected, "span equals expected bytes") {
    if (arg.size() != expected.size()) return false;
    return std::equal(arg.begin(), arg.end(), expected.begin());
}

class MasterBridgeTest : public ::testing::Test
{
protected:
    MockIEspNow    mockEspNow;
    MasterBridge   bridge{mockEspNow};
};

TEST_F(MasterBridgeTest, InitDelegatesToEspNow)
{
    EXPECT_CALL(mockEspNow, init()).WillOnce(Return(ESP_OK));
    EXPECT_EQ(ESP_OK, bridge.init());
}

TEST_F(MasterBridgeTest, InitPropagatesError)
{
    EXPECT_CALL(mockEspNow, init()).WillOnce(Return(ESP_FAIL));
    EXPECT_EQ(ESP_FAIL, bridge.init());
}

TEST_F(MasterBridgeTest, SendMessageDelegatesToEspNow)
{
    const uint8_t mac[6] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
    const std::vector<uint8_t> data = {0xAA, 0xBB, 0xCC};

    EXPECT_CALL(mockEspNow, sendMessage(SpanEq(std::span<const uint8_t>{data})))
        .WillOnce(Return(ESP_OK));
    EXPECT_EQ(ESP_OK, bridge.sendMessage(mac, std::span<const uint8_t>{data}));
}

TEST_F(MasterBridgeTest, SendMessagePropagatesError)
{
    const uint8_t mac[6] = {};
    const std::vector<uint8_t> data = {0x01};

    EXPECT_CALL(mockEspNow, sendMessage(_)).WillOnce(Return(ESP_FAIL));
    EXPECT_EQ(ESP_FAIL, bridge.sendMessage(mac, std::span<const uint8_t>{data}));
}

TEST_F(MasterBridgeTest, BroadcastMessageDelegatesToEspNow)
{
    const std::vector<uint8_t> data = {0x10, 0x20};

    EXPECT_CALL(mockEspNow, sendMessage(SpanEq(std::span<const uint8_t>{data})))
        .WillOnce(Return(ESP_OK));
    EXPECT_EQ(ESP_OK, bridge.broadcastMessage(std::span<const uint8_t>{data}));
}

TEST_F(MasterBridgeTest, AddSlaveDelegatesToConnectPeer)
{
    esp_now_peer_info_t peer{};
    peer.peer_addr[0] = 0xAA;

    EXPECT_CALL(mockEspNow, connectPeer(&peer)).WillOnce(Return(ESP_OK));
    EXPECT_EQ(ESP_OK, bridge.addSlave(peer));
}

TEST_F(MasterBridgeTest, AddSlavePropagatesError)
{
    esp_now_peer_info_t peer{};
    EXPECT_CALL(mockEspNow, connectPeer(_)).WillOnce(Return(ESP_FAIL));
    EXPECT_EQ(ESP_FAIL, bridge.addSlave(peer));
}
