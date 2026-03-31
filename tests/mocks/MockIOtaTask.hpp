#pragma once
#include <gmock/gmock.h>
#include "IOtaTask.hpp"

// IOtaTask has no abstract methods beyond the destructor.
class MockIOtaTask : public IOtaTask {};
