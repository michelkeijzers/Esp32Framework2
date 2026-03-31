#pragma once
#include <gmock/gmock.h>
#include "common/context/IEspNowFactory.hpp"
#include "MockIEspNow.hpp"

class MockEspNowFactory : public IEspNowFactory
{
public:
    ::testing::NiceMock<MockIEspNow> espNow_;

    IEspNow &getEspNow() override { return espNow_; }
};
