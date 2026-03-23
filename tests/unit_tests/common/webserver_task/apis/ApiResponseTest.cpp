#include <gtest/gtest.h>
#include "ApiResponse.hpp"

class ApiResponseTest : public ::testing::Test
{
protected:
    char   buf[512]{};
    size_t bufSize = sizeof(buf);
};

// ─────────────────────────────────────────────────────────────
// build_success_response
// ─────────────────────────────────────────────────────────────

TEST_F(ApiResponseTest, SuccessResponseBasic)
{
    int n = ApiResponse::build_success_response(buf, bufSize, "{}");
    ASSERT_GT(n, 0);
    EXPECT_STREQ("{\"success\":true,\"data\":{}}", buf);
}

TEST_F(ApiResponseTest, SuccessResponseWithArray)
{
    int n = ApiResponse::build_success_response(buf, bufSize, "[1,2,3]");
    ASSERT_GT(n, 0);
    EXPECT_STREQ("{\"success\":true,\"data\":[1,2,3]}", buf);
}

TEST_F(ApiResponseTest, SuccessResponseNullBufferReturnsNegative)
{
    int n = ApiResponse::build_success_response(nullptr, bufSize, "{}");
    EXPECT_LT(n, 0);
}

TEST_F(ApiResponseTest, SuccessResponseNullDataReturnsNegative)
{
    int n = ApiResponse::build_success_response(buf, bufSize, nullptr);
    EXPECT_LT(n, 0);
}

TEST_F(ApiResponseTest, SuccessResponseBufferTooSmallReturnsNegative)
{
    char small[10]{};
    int n = ApiResponse::build_success_response(small, sizeof(small), "{}");
    EXPECT_LT(n, 0);
}

// ─────────────────────────────────────────────────────────────
// build_error_response
// ─────────────────────────────────────────────────────────────

TEST_F(ApiResponseTest, ErrorResponseBasic)
{
    int n = ApiResponse::build_error_response(
        buf, bufSize, ApiResponse::NOT_FOUND, "not found");
    ASSERT_GT(n, 0);
    EXPECT_STREQ("{\"success\":false,\"error\":{\"code\":404,\"message\":\"not found\"}}", buf);
}

TEST_F(ApiResponseTest, ErrorResponseBadRequest)
{
    int n = ApiResponse::build_error_response(
        buf, bufSize, ApiResponse::BAD_REQUEST, "invalid input");
    ASSERT_GT(n, 0);
    EXPECT_STREQ("{\"success\":false,\"error\":{\"code\":400,\"message\":\"invalid input\"}}", buf);
}

TEST_F(ApiResponseTest, ErrorResponseNullBufferReturnsNegative)
{
    int n = ApiResponse::build_error_response(
        nullptr, bufSize, ApiResponse::OK, "msg");
    EXPECT_LT(n, 0);
}

TEST_F(ApiResponseTest, ErrorResponseNullMessageReturnsNegative)
{
    int n = ApiResponse::build_error_response(buf, bufSize, ApiResponse::OK, nullptr);
    EXPECT_LT(n, 0);
}

TEST_F(ApiResponseTest, ErrorResponseBufferTooSmallReturnsNegative)
{
    char small[20]{};
    int n = ApiResponse::build_error_response(
        small, sizeof(small), ApiResponse::OK, "msg");
    EXPECT_LT(n, 0);
}

// ─────────────────────────────────────────────────────────────
// get_http_status
// ─────────────────────────────────────────────────────────────

TEST_F(ApiResponseTest, GetHttpStatusOk)      { EXPECT_EQ(200, ApiResponse::get_http_status(ApiResponse::OK)); }
TEST_F(ApiResponseTest, GetHttpStatusCreated) { EXPECT_EQ(201, ApiResponse::get_http_status(ApiResponse::CREATED)); }
TEST_F(ApiResponseTest, GetHttpStatus400)     { EXPECT_EQ(400, ApiResponse::get_http_status(ApiResponse::BAD_REQUEST)); }
TEST_F(ApiResponseTest, GetHttpStatus404)     { EXPECT_EQ(404, ApiResponse::get_http_status(ApiResponse::NOT_FOUND)); }
TEST_F(ApiResponseTest, GetHttpStatus500)     { EXPECT_EQ(500, ApiResponse::get_http_status(ApiResponse::INTERNAL_SERVER_ERROR)); }

// ─────────────────────────────────────────────────────────────
// get_status_text
// ─────────────────────────────────────────────────────────────

TEST_F(ApiResponseTest, GetStatusTextOk)      { EXPECT_STREQ("OK",                   ApiResponse::get_status_text(ApiResponse::OK)); }
TEST_F(ApiResponseTest, GetStatusTextCreated) { EXPECT_STREQ("Created",              ApiResponse::get_status_text(ApiResponse::CREATED)); }
TEST_F(ApiResponseTest, GetStatusText400)     { EXPECT_STREQ("Bad Request",          ApiResponse::get_status_text(ApiResponse::BAD_REQUEST)); }
TEST_F(ApiResponseTest, GetStatusText404)     { EXPECT_STREQ("Not Found",            ApiResponse::get_status_text(ApiResponse::NOT_FOUND)); }
TEST_F(ApiResponseTest, GetStatusText500)     { EXPECT_STREQ("Internal Server Error",ApiResponse::get_status_text(ApiResponse::INTERNAL_SERVER_ERROR)); }
