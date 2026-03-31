#include "ApiResponse.hpp"

int ApiResponse::build_success_response(char* buffer, size_t buffer_size, const char* data) {
    if (!buffer || buffer_size < 50 || !data) {
        return -1;
    }
    
    return snprintf(buffer, buffer_size, 
                    "{\"success\":true,\"data\":%s}", 
                    data);
}

int ApiResponse::build_error_response(char* buffer, size_t buffer_size, 
                                      StatusCode status_code, const char* error_message) {
    if (!buffer || buffer_size < 100 || !error_message) {
        return -1;
    }
    
    int status = get_http_status(status_code);
    return snprintf(buffer, buffer_size, 
                    "{\"success\":false,\"error\":{\"code\":%d,\"message\":\"%s\"}}", 
                    status, error_message);
}

int ApiResponse::get_http_status(StatusCode code) {
    return static_cast<int>(code);
}

const char* ApiResponse::get_status_text(StatusCode code) {
    switch (code) {
        case OK: return "OK";
        case CREATED: return "Created";
        case BAD_REQUEST: return "Bad Request";
        case UNAUTHORIZED: return "Unauthorized";
        case FORBIDDEN: return "Forbidden";
        case NOT_FOUND: return "Not Found";
        case CONFLICT: return "Conflict";
        case INTERNAL_SERVER_ERROR: return "Internal Server Error";
        default: return "Unknown";
    }
}
