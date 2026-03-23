#pragma once

#include "IApiServer.hpp"
#include <memory>
#include "../esp/esp_http_server/esp_http_server_if.hpp"
#include "apis/StaticFileHandler.hpp"
#include "apis/ApiStatus.hpp"
#include "apis/ApiNodes.hpp"
#include "apis/ApiSystem.hpp"
#include "apis/ApiFirmware.hpp"
#include "apis/ApiSecurity.hpp"
#include "apis/ApiLogging.hpp"

#include "../context/Contexts.hpp"

class IEspLittleFs;
class IEspHttpServer;
class IEspLogger;

/**
 * Base API Server class with common functionality for all API server implementations
 * Handles LittleFS mounting, static file serving, and generic API registration
 * Derived classes can override register_endpoints() to add project-specific endpoints
 */

class ApiServer : public IApiServer
{
public:
  explicit ApiServer(Contexts &contexts);
  virtual ~ApiServer();

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
    void register_generic_endpoints();
    
    /**
     * Register endpoints - override in derived classes to add project-specific endpoints
     */
    virtual void register_endpoints();

private:
    /**
     * Handler for static files (internal implementation)
     */
    esp_err_t static_file_handler(httpd_req_t *req);
};
