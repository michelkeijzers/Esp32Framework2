#include "LowLevelStubCounters.hpp"

namespace low_level_stubs {
Counters &counters() {
    static Counters state;
    return state;
}

void reset_counters() { counters() = Counters{}; }
}  // namespace low_level_stubs
