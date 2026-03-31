#pragma once
#include <gmock/gmock.h>
#include "project_dmx_controller/web_server_task/IApiServer.hpp"

class MockIApiServer : public IApiServer
{
public:
    MOCK_METHOD(void, start, (), (override));
    MOCK_METHOD(void, stop, (), (override));
};
