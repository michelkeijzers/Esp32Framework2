#pragma once
#include <gmock/gmock.h>

#include "IEspLogger.hpp"

class MockIEspLogger : public IEspLogger {
   public:
    MOCK_METHOD(void, log_error, (const char *, const char *), (override));
    MOCK_METHOD(void, log_info, (const char *, const char *), (override));
    MOCK_METHOD(void, log_warn, (const char *, const char *), (override));
};
