#pragma once
#include <gmock/gmock.h>
#include "ILoggingTask.hpp"

// ILoggingTask has no abstract methods beyond the destructor.
// This stub satisfies the interface for use as a reference.
class MockILoggingTask : public ILoggingTask {};
