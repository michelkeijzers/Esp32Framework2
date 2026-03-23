#pragma once
#include <gmock/gmock.h>
#include "IApiServer.hpp"

class MockIApiServer : public IApiServer
{
public:
    MOCK_METHOD(void, start, (), (override));
    MOCK_METHOD(void, stop, (), (override));
};
