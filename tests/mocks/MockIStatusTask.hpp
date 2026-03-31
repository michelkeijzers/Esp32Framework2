#pragma once
#include <gmock/gmock.h>
#include "IStatusTask.hpp"

// IStatusTask has no abstract methods beyond the destructor.
class MockIStatusTask : public IStatusTask {};
