#include <gtest/gtest.h>

#include "MockIFreeRtosFactory.hpp"
#include "project_dmx_controller/function_tasks/max7219_task/Max7219Task.hpp"

struct Max7219TaskTest : public ::testing::Test {
    ::testing::NiceMock<MockIFreeRtosFactory> mockRtosFactory;
    Max7219Task task{mockRtosFactory};
};

TEST_F(Max7219TaskTest, InitReturnsOk) { EXPECT_EQ(ESP_OK, task.init()); }

TEST_F(Max7219TaskTest, StartReturnsOk) {
    task.init();
    EXPECT_EQ(ESP_OK, task.start());
}