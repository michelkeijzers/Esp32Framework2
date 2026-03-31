#pragma once
#include <string>
#include <cstring>

class StaticFileHandler {
public:
    // Get MIME type based on file extension
    static const char* get_mime_type(const char* path) {
        if (!path) return "application/octet-stream";
        
        // Check file extension
        if (strstr(path, ".html")) return "text/html";
        if (strstr(path, ".htm"))  return "text/html";
        if (strstr(path, ".css"))  return "text/css";
        if (strstr(path, ".js"))   return "application/javascript";
        if (strstr(path, ".json")) return "application/json";
        if (strstr(path, ".png"))  return "image/png";
        if (strstr(path, ".jpg"))  return "image/jpeg";
        if (strstr(path, ".jpeg")) return "image/jpeg";
        if (strstr(path, ".gif"))  return "image/gif";
        if (strstr(path, ".svg"))  return "image/svg+xml";
        if (strstr(path, ".ico"))  return "image/x-icon";
        if (strstr(path, ".txt"))  return "text/plain";
        if (strstr(path, ".woff")) return "font/woff";
        if (strstr(path, ".woff2")) return "font/woff2";
        
        // Default
        return "application/octet-stream";
    }
};
