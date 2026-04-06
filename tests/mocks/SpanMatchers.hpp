#pragma once

#include <gmock/gmock.h>
#include <span>
#include <algorithm>

/**
 * GMock matcher that compares a std::span<const uint8_t> against an expected
 * sequence for byte-by-byte equality.
 *
 * Usage:
 * @code
 *   std::vector<uint8_t> data = {0xAA, 0xBB};
 *   EXPECT_CALL(mock, sendMessage(SpanEq(std::span<const uint8_t>{data})));
 * @endcode
 */
MATCHER_P(SpanEq, expected, "span equals expected bytes") {
    if (arg.size() != expected.size()) return false;
    return std::equal(arg.begin(), arg.end(), expected.begin());
}
