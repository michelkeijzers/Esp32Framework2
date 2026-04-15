#include <gtest/gtest.h>

#include "MockIFreeRtosFactory.hpp"
#include "project_dmx_controller/function_tasks/potmeter_task/PotmeterTask.hpp"

struct PotmeterTaskTest : public ::testing::Test {
    ::testing::NiceMock<MockIFreeRtosFactory> mockRtosFactory;
    PotmeterTask task{mockRtosFactory, 3};
};

TEST_F(PotmeterTaskTest, InitReturnsOk) { EXPECT_EQ(ESP_OK, task.init()); }

TEST_F(PotmeterTaskTest, StartReturnsOk) {
    task.init();
    EXPECT_EQ(ESP_OK, task.start());
}