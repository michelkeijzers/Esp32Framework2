#include <gtest/gtest.h>

#include "MockIFreeRtosFactory.hpp"
#include "project_dmx_controller/function_tasks/button_task/ButtonTask.hpp"

struct ButtonTaskTest : public ::testing::Test {
    ::testing::NiceMock<MockIFreeRtosFactory> mockRtosFactory;
    ButtonTask task{mockRtosFactory, 12};
};

TEST_F(ButtonTaskTest, InitReturnsOk) { EXPECT_EQ(ESP_OK, task.init()); }

TEST_F(ButtonTaskTest, StartReturnsOk) {
    task.init();
    EXPECT_EQ(ESP_OK, task.start());
}