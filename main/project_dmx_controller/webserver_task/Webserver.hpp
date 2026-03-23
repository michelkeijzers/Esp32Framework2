#pragma once

#include "IWebserver.hpp"
#include <memory>
#include "../../../common/esp/esp_http_server/esp_http_server_if.hpp"
#include "../apis/StaticFileHandler.hpp"
#include "../apis/ApiStatus.hpp"
#include "../apis/ApiNodes.hpp"
#include "../apis/ApiSystem.hpp"
#include "../apis/ApiFirmware.hpp"
#include "../apis/ApiSecurity.hpp"
#include "../apis/ApiLogging.hpp"

#include "../../../common/context/Contexts.hpp"

class IEspLittleFs;
class IEspHttpServer;
class IEspLogger;

/**
 * Base Webserver class with common functionality for all webserver implementations
 * Handles LittleFS mounting, static file serving, and generic API registration
 * Derived classes can override register_endpoints() to add project-specific endpoints
 */

class Webserver : public IWebserver
{
public:
  explicit Webserver(Contexts &contexts);
  virtual ~Webserver();

  virtual void start() override;
  virtual void stop() override;

  // Thunk for static file handler
  static esp_err_t static_file_handler_thunk(httpd_req_t *req);

protected:
    // Order matters for -Werror=reorder: server must come before API pointers

    IEspLittleFs& espLittleFs_;
    IEspHttpServer& espHttpServer_;
    IEspLogger& logger_;

    IApiStatus& apiStatus_;
    IApiNodes& apiNodes_;
    IApiSystem& apiSystem_;
    IApiFirmware& apiFirmware_;
    IApiSecurity& apiSecurity_;
    IApiLogging& apiLogging_;

    httpd_handle_t server;

    void mount_littlefs();
    void register_static_file_handler();
    
    /**
     * Virtual method to register endpoints
     * Base implementation registers generic APIs and static file handler
     * Derived classes can override to add project-specific endpoints
     */
    virtual void register_endpoints();
    virtual void register_generic_endpoints();

    // Non-static member for static file handler
    esp_err_t static_file_handler(httpd_req_t *req);

private:
};
