#pragma once

#ifdef ESP_PLATFORM
#include "esp_http_server.h"
#else
#include <cstdint>
#include <string>

inline constexpr int kHttpdSockErrFail = -1;
inline constexpr int kHttpdSockErrInvalid = -2;
inline constexpr int kHttpdSockErrTimeout = -3;

#define HTTPD_SOCK_ERR_FAIL (kHttpdSockErrFail)
#define HTTPD_SOCK_ERR_INVALID (kHttpdSockErrInvalid)
#define HTTPD_SOCK_ERR_TIMEOUT (kHttpdSockErrTimeout)

/* Request Methods */
#define HTTP_METHOD_MAP(XX)          \
    XX(0, DELETE, DELETE)            \
    XX(1, GET, GET)                  \
    XX(2, HEAD, HEAD)                \
    XX(3, POST, POST)                \
    XX(4, PUT, PUT)                  \
    /* pathological */               \
    XX(5, CONNECT, CONNECT)          \
    XX(6, OPTIONS, OPTIONS)          \
    XX(7, TRACE, TRACE)              \
    /* WebDAV */                     \
    XX(8, COPY, COPY)                \
    XX(9, LOCK, LOCK)                \
    XX(10, MKCOL, MKCOL)             \
    XX(11, MOVE, MOVE)               \
    XX(12, PROPFIND, PROPFIND)       \
    XX(13, PROPPATCH, PROPPATCH)     \
    XX(14, SEARCH, SEARCH)           \
    XX(15, UNLOCK, UNLOCK)           \
    XX(16, BIND, BIND)               \
    XX(17, REBIND, REBIND)           \
    XX(18, UNBIND, UNBIND)           \
    XX(19, ACL, ACL)                 \
    /* subversion */                 \
    XX(20, REPORT, REPORT)           \
    XX(21, MKACTIVITY, MKACTIVITY)   \
    XX(22, CHECKOUT, CHECKOUT)       \
    XX(23, MERGE, MERGE)             \
    /* upnp */                       \
    XX(24, MSEARCH, M - SEARCH)      \
    XX(25, NOTIFY, NOTIFY)           \
    XX(26, SUBSCRIBE, SUBSCRIBE)     \
    XX(27, UNSUBSCRIBE, UNSUBSCRIBE) \
    /* RFC-5789 */                   \
    XX(28, PATCH, PATCH)             \
    XX(29, PURGE, PURGE)             \
    /* CalDAV */                     \
    XX(30, MKCALENDAR, MKCALENDAR)   \
    /* RFC-2068, section 19.6.1.2 */ \
    XX(31, LINK, LINK)               \
    XX(32, UNLINK, UNLINK)

enum http_method : std::uint8_t {
#define XX(num, name, string) HTTP_##name = (num),
    HTTP_METHOD_MAP(XX)
#undef XX
};

using esp_err_t = int;

typedef enum http_method httpd_method_t;

typedef void* httpd_handle_t;

struct httpd_config_t {};

inline httpd_config_t HTTPD_DEFAULT_CONFIG() { return httpd_config_t(); }

inline constexpr int kHttpdRespUseStrlen = -1;

#define HTTPD_RESP_USE_STRLEN (kHttpdRespUseStrlen)

struct httpd_req_t {
    void* user_ctx;
    const char* uri;
    std::string response;
    std::string content_type;
};

using httpd_uri_handler_t = esp_err_t (*)(httpd_req_t* request);

typedef struct httpd_uri {
    const char* uri;
    httpd_method_t method;
    httpd_uri_handler_t handler;
    void* user_ctx;
    bool is_websocket;
    bool handle_ws_control_frames;
    const char* supported_subprotocol;
} httpd_uri_t;
#endif
