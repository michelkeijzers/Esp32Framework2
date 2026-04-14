#include <gtest/gtest.h>

#include "UriParamExtractor.hpp"

// ─────────────────────────────────────────────────────────────
// extractParams
// ─────────────────────────────────────────────────────────────

TEST(UriParamExtractorTest, SingleWildcardMatches) {
    auto params = UriParamExtractor::extractParams("/api/v1/presets/*", "/api/v1/presets/42");
    ASSERT_EQ(1u, params.size());
    EXPECT_EQ("42", params[0]);
}

TEST(UriParamExtractorTest, TwoWildcardsMatch) {
    auto params = UriParamExtractor::extractParams("/api/v1/preset/*/channel/*",
                                                   "/api/v1/preset/7/channel/255");
    ASSERT_EQ(2u, params.size());
    EXPECT_EQ("7", params[0]);
    EXPECT_EQ("255", params[1]);
}

TEST(UriParamExtractorTest, NoMatchReturnsEmpty) {
    auto params = UriParamExtractor::extractParams("/api/v1/presets/*", "/other/path");
    EXPECT_TRUE(params.empty());
}

TEST(UriParamExtractorTest, ExactMatchNoWildcardReturnsEmpty) {
    auto params = UriParamExtractor::extractParams("/api/v1/status", "/api/v1/status");
    EXPECT_TRUE(params.empty());
}

// ─────────────────────────────────────────────────────────────
// extractFirstParam
// ─────────────────────────────────────────────────────────────

TEST(UriParamExtractorTest, ExtractFirstParamFound) {
    auto p = UriParamExtractor::extractFirstParam("/api/v1/presets/*", "/api/v1/presets/99");
    EXPECT_EQ("99", p);
}

TEST(UriParamExtractorTest, ExtractFirstParamNotFoundReturnsEmpty) {
    auto p = UriParamExtractor::extractFirstParam("/api/v1/presets/*", "/wrong/path");
    EXPECT_TRUE(p.empty());
}

// ─────────────────────────────────────────────────────────────
// extractFirstParamAsUint32
// ─────────────────────────────────────────────────────────────

TEST(UriParamExtractorTest, ExtractUint32ValidNumber) {
    uint32_t v =
        UriParamExtractor::extractFirstParamAsUint32("/api/v1/presets/*", "/api/v1/presets/123");
    EXPECT_EQ(123u, v);
}

TEST(UriParamExtractorTest, ExtractUint32ZeroParam) {
    uint32_t v =
        UriParamExtractor::extractFirstParamAsUint32("/api/v1/presets/*", "/api/v1/presets/0");
    EXPECT_EQ(0u, v);
}

TEST(UriParamExtractorTest, ExtractUint32NoMatchReturnsZero) {
    uint32_t v = UriParamExtractor::extractFirstParamAsUint32("/api/v1/presets/*", "/other/path");
    EXPECT_EQ(0u, v);
}

TEST(UriParamExtractorTest, ExtractUint32NonNumericReturnsZero) {
    uint32_t v =
        UriParamExtractor::extractFirstParamAsUint32("/api/v1/presets/*", "/api/v1/presets/abc");
    EXPECT_EQ(0u, v);
}

TEST(UriParamExtractorTest, ExtractUint32LargeNumber) {
    uint32_t v =
        UriParamExtractor::extractFirstParamAsUint32("/api/v1/presets/*", "/api/v1/presets/65535");
    EXPECT_EQ(65535u, v);
}
