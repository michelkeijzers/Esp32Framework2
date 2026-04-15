#include <gtest/gtest.h>

#include "MockIFreeRtosFactory.hpp"
#include "project_dmx_controller/function_tasks/gpio_led_task/GpioLedTask.hpp"

struct GpioLedTaskTest : public ::testing::Test {
    ::testing::NiceMock<MockIFreeRtosFactory> mockRtosFactory;
    GpioLedTask task{mockRtosFactory, 5};
};

TEST_F(GpioLedTaskTest, InitReturnsOk) { EXPECT_EQ(ESP_OK, task.init()); }

TEST_F(GpioLedTaskTest, StartReturnsOk) {
    task.init();
    EXPECT_EQ(ESP_OK, task.start());
}