#pragma once
#include <cstdio>
#include <cstring>

/**
 * Standard API Response Format Utility
 *
 * Provides helper methods for building consistent JSON responses across all API endpoints.
 *
 * Success Response Format:
 * {"success": true, "data": {...}}
 *
 * Error Response Format:
 * {"success": false, "error": {"code": 400, "message": "Bad Request"}}
 */
class ApiResponse {
   public:
    // HTTP Status Codes
    enum StatusCode {
        OK = 200,
        CREATED = 201,
        BAD_REQUEST = 400,
        UNAUTHORIZED = 401,
        FORBIDDEN = 403,
        NOT_FOUND = 404,
        CONFLICT = 409,
        INTERNAL_SERVER_ERROR = 500
    };

    /**
     * Build a success response JSON string
     * @param buffer - Output buffer for JSON string
     * @param buffer_size - Size of output buffer
     * @param data - JSON data string (no outer braces needed)
     * @return Number of bytes written, or -1 on error
     */
    static int build_success_response(char* buffer, size_t buffer_size, const char* data);

    /**
     * Build an error response JSON string
     * @param buffer - Output buffer for JSON string
     * @param buffer_size - Size of output buffer
     * @param status_code - HTTP status code
     * @param error_message - Human-readable error message
     * @return Number of bytes written, or -1 on error
     */
    static int build_error_response(char* buffer, size_t buffer_size, StatusCode status_code,
                                    const char* error_message);

    /**
     * Get HTTP status code integer
     * @param code - StatusCode enum
     * @return HTTP status code as integer
     */
    static int get_http_status(StatusCode code);

    /**
     * Get human-readable status text
     * @param code - StatusCode enum
     * @return Status text string (e.g., "Bad Request")
     */
    static const char* get_status_text(StatusCode code);
};
