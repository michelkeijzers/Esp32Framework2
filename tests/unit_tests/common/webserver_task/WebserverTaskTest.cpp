#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "project_dmx_controller/webserver_task/WebserverTask.hpp"
#include "MockIApiServer.hpp"
#include "MockIFreeRtosFactory.hpp"

using ::testing::Return;

class WebserverTaskTest : public ::testing::Test
{
protected:
    ::testing::NiceMock<MockIFreeRtosFactory> mockRtosFactory;
    MockIApiServer  mockApiServer;
    WebserverTask   task{mockRtosFactory, mockApiServer, "webserver_task", 8192, 6};
};

TEST_F(WebserverTaskTest, InitReturnsOk)
{
    EXPECT_EQ(ESP_OK, task.init());
}

TEST_F(WebserverTaskTest, StartCallsApiServerStart)
{
    EXPECT_CALL(mockApiServer, start()).Times(1);
    EXPECT_EQ(ESP_OK, task.start());
}

TEST_F(WebserverTaskTest, StopCallsApiServerStop)
{
    EXPECT_CALL(mockApiServer, stop()).Times(1);
    EXPECT_EQ(ESP_OK, task.stop());
}

TEST_F(WebserverTaskTest, InitDoesNotCallApiServer)
{
    EXPECT_CALL(mockApiServer, start()).Times(0);
    EXPECT_CALL(mockApiServer, stop()).Times(0);
    EXPECT_EQ(ESP_OK, task.init());
}
