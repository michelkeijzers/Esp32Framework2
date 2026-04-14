#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <span>
#include <vector>

#include "MasterBridge.hpp"
#include "MockIEspNow.hpp"
#include "SlaveBridge.hpp"
#include "SpanMatchers.hpp"

using ::testing::_;
using ::testing::Return;

TEST(BridgeIntegrationTest, MasterAndSlaveInitAgainstEspNow) {
    MockIEspNow espNow;

    MasterBridge masterBridge(espNow);
    SlaveBridge slaveBridge(espNow);

    EXPECT_CALL(espNow, init()).Times(2).WillRepeatedly(Return(ESP_OK));

    EXPECT_EQ(ESP_OK, masterBridge.init());
    EXPECT_EQ(ESP_OK, slaveBridge.init());
}

TEST(BridgeIntegrationTest, MasterOperationsDelegateToEspNow) {
    MockIEspNow espNow;
    MasterBridge masterBridge(espNow);

    esp_now_peer_info_t peer{};
    peer.peer_addr[0] = 1;
    peer.peer_addr[1] = 2;
    peer.peer_addr[2] = 3;
    peer.peer_addr[3] = 4;
    peer.peer_addr[4] = 5;
    peer.peer_addr[5] = 6;
    std::vector<uint8_t> payload = {9, 8, 7};

    EXPECT_CALL(espNow, init()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(espNow, connectPeer(_)).WillOnce(Return(ESP_OK));
    EXPECT_CALL(espNow, sendMessage(SpanEq(std::span<const uint8_t>{payload})))
        .WillOnce(Return(ESP_OK));

    EXPECT_EQ(ESP_OK, masterBridge.init());
    EXPECT_EQ(ESP_OK, masterBridge.addSlave(peer));
    EXPECT_EQ(ESP_OK, masterBridge.sendMessage(peer.peer_addr, std::span<const uint8_t>{payload}));
}

TEST(BridgeIntegrationTest, SlaveRegistersMasterAndReceiveCallback) {
    MockIEspNow espNow;
    SlaveBridge slaveBridge(espNow);

    esp_now_peer_info_t master{};
    master.peer_addr[0] = 6;
    master.peer_addr[1] = 5;
    master.peer_addr[2] = 4;
    master.peer_addr[3] = 3;
    master.peer_addr[4] = 2;
    master.peer_addr[5] = 1;
    auto callback = [](std::span<const uint8_t>) {};

    EXPECT_CALL(espNow, init()).WillOnce(Return(ESP_OK));
    EXPECT_CALL(espNow, connectPeer(_)).WillOnce(Return(ESP_OK));

    EXPECT_EQ(ESP_OK, slaveBridge.init());
    EXPECT_EQ(ESP_OK, slaveBridge.registerMaster(master));
    slaveBridge.setReceiveCallback(callback);
    SUCCEED();
}
