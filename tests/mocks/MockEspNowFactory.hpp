#pragma once
#include <gmock/gmock.h>

#include "MockIEspNow.hpp"
#include "common/context/IEspNowFactory.hpp"

class MockEspNowFactory : public IEspNowFactory {
   public:
    ::testing::NiceMock<MockIEspNow> espNow_;

    IEspNow &getEspNow() override { return espNow_; }
};
