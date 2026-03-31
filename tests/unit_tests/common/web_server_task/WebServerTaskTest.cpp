#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "project_dmx_controller/web_server_task/WebServerTask.hpp"
#include "MockIApiServer.hpp"
#include "MockIFreeRtosFactory.hpp"

using ::testing::Return;

class WebServerTaskTest : public ::testing::Test
{
protected:
    ::testing::NiceMock<MockIFreeRtosFactory> mockRtosFactory;
    MockIApiServer mockApiServer;
    WebServerTask task{mockRtosFactory, mockApiServer, "web_server_task", 8192, 6};
};

TEST_F(WebServerTaskTest, InitReturnsOk)
{
    EXPECT_EQ(ESP_OK, task.init());
}

TEST_F(WebServerTaskTest, StartCallsApiServerStart)
{
    EXPECT_CALL(mockApiServer, start()).Times(1);
    EXPECT_EQ(ESP_OK, task.start());
}

TEST_F(WebServerTaskTest, StopCallsApiServerStop)
{
    EXPECT_CALL(mockApiServer, stop()).Times(1);
    EXPECT_EQ(ESP_OK, task.stop());
}

TEST_F(WebServerTaskTest, InitDoesNotCallApiServer)
{
    EXPECT_CALL(mockApiServer, start()).Times(0);
    EXPECT_CALL(mockApiServer, stop()).Times(0);
    EXPECT_EQ(ESP_OK, task.init());
}
