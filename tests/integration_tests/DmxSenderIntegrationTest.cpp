#include <gtest/gtest.h>

#include "DmxMax3485SenderTask.hpp"
#include "MockIFreeRtosFactory.hpp"

TEST(DmxSenderIntegrationTest, FullChannelProgrammingWorkflow) {
    ::testing::NiceMock<MockIFreeRtosFactory> mockRtosFactory;
    DmxMax3485SenderTask task(mockRtosFactory);

    EXPECT_EQ(ESP_OK, task.init());
    EXPECT_EQ(ESP_OK, task.start());

    for (uint16_t channel = 0; channel < 512; ++channel) {
        task.setChannelValue(channel, static_cast<uint8_t>(channel % 256));
    }

    uint8_t frame[512];
    for (int i = 0; i < 512; ++i) {
        frame[i] = static_cast<uint8_t>((255 - i) & 0xFF);
    }

    task.setAllChannelValues(frame);

    SUCCEED();
}
