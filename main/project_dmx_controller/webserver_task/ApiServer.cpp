#include "ApiServer.hpp"
#include "../../../common/webserver_task/apis/ApiStatus.hpp"
#include "../../../common/webserver_task/apis/ApiNodes.hpp"
#include "../../../common/webserver_task/apis/ApiSystem.hpp"
#include "../../../common/webserver_task/apis/ApiFirmware.hpp"
#include "../../../common/webserver_task/apis/ApiSecurity.hpp"
#include "../../../common/webserver_task/apis/ApiLogging.hpp"
#include "../../../common/webserver_task/apis/StaticFileHandler.hpp"
#include "../../../common/esp/esp_logger/IEspLogger.hpp"
#include "../../../common/esp/esp_http_server/IEspHttpServer.hpp"
#include "../../../common/esp/esp_file_systems/IEspLittleFs.hpp"

#include <cstring>

ApiServer::ApiServer(Contexts &contexts)
    : espLittleFs_(contexts.espContexts.espLittleFs), espHttpServer_(contexts.espContexts.espHttpServer),
      logger_(contexts.espContexts.espLogger), apiStatus_(contexts.commonApiContexts.apiStatus),
      apiNodes_(contexts.commonApiContexts.apiNodes), apiSystem_(contexts.commonApiContexts.apiSystem),
      apiFirmware_(contexts.commonApiContexts.apiFirmware), apiSecurity_(contexts.commonApiContexts.apiSecurity),
      apiLogging_(contexts.commonApiContexts.apiLogging), server(nullptr)
{
    server = nullptr;
}

ApiServer::~ApiServer()
{
    stop();
}

void ApiServer::start()
{
    mount_littlefs();

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    if (espHttpServer_.httpd_start(&server, &config) == ESP_OK)
    {
        register_endpoints();
    }
    else
    {
        logger_.log_error("ApiServer", "Failed to start HTTP server");
    }
}

void ApiServer::stop()
{
    if (server)
    {
        espHttpServer_.httpd_stop(server);
        server = NULL;
    }
}

esp_err_t ApiServer::static_file_handler(httpd_req_t *req)
{
    char filepath[1024];
    snprintf(filepath, sizeof(filepath), "/littlefs%s", req->uri);
    if (strcmp(req->uri, "/") == 0)
    {
        snprintf(filepath, sizeof(filepath), "/littlefs/index.html");
    }

    FILE *f = fopen(filepath, "r");
    if (!f)
    {
        if (strcmp(filepath, "/littlefs/index.html") != 0)
        {
            snprintf(filepath, sizeof(filepath), "/littlefs/index.html");
            f = fopen(filepath, "r");
        }
        if (!f)
        {
            espHttpServer_.httpd_resp_send_404(req);
            return ESP_OK;
        }
    }

    const char *mime_type = StaticFileHandler::get_mime_type(filepath);
    espHttpServer_.httpd_resp_set_type(req, mime_type);

    char buffer[1024];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), f)) > 0)
    {
        espHttpServer_.httpd_resp_send_chunk(req, buffer, bytes_read);
    }
    espHttpServer_.httpd_resp_send_chunk(req, NULL, 0);
    fclose(f);
    return ESP_OK;
}

esp_err_t ApiServer::static_file_handler_thunk(httpd_req_t *req)
{
    auto *self = static_cast<ApiServer *>(req->user_ctx);
    return self->static_file_handler(req);
}

void ApiServer::register_endpoints()
{
    register_generic_endpoints();
    register_static_file_handler();
}

static esp_err_t status_handler_static(httpd_req_t *req)
{
    auto *obj = static_cast<IApiStatus *>(req->user_ctx);
    return obj->get_status_handler(req);
}
static esp_err_t status_stream_handler_static(httpd_req_t *req)
{
    auto *obj = static_cast<IApiStatus *>(req->user_ctx);
    return obj->get_status_stream_handler(req);
}

static esp_err_t nodes_info_handler_static(httpd_req_t *req)
{
    auto *obj = static_cast<IApiNodes *>(req->user_ctx);
    return obj->get_nodes_info_handler(req);
}
static esp_err_t save_nodes_info_handler_static(httpd_req_t *req)
{
    auto *obj = static_cast<IApiNodes *>(req->user_ctx);
    return obj->save_nodes_info_handler(req);
}

static esp_err_t reboot_handler_static(httpd_req_t *req)
{
    auto *obj = static_cast<IApiSystem *>(req->user_ctx);
    return obj->reboot_handler(req);
}
static esp_err_t factory_reset_handler_static(httpd_req_t *req)
{
    auto *obj = static_cast<IApiSystem *>(req->user_ctx);
    return obj->factory_reset_handler(req);
}

static esp_err_t firmware_chunk_handler_static(httpd_req_t *req)
{
    auto *obj = static_cast<IApiFirmware *>(req->user_ctx);
    return obj->firmware_chunk_handler(req);
}
static esp_err_t firmware_finish_handler_static(httpd_req_t *req)
{
    auto *obj = static_cast<IApiFirmware *>(req->user_ctx);
    return obj->firmware_finish_handler(req);
}

static esp_err_t esp_now_key_handler_static(httpd_req_t *req)
{
    auto *obj = static_cast<IApiSecurity *>(req->user_ctx);
    return obj->esp_now_key_handler(req);
}
static esp_err_t wifi_password_handler_static(httpd_req_t *req)
{
    auto *obj = static_cast<IApiSecurity *>(req->user_ctx);
    return obj->wifi_password_handler(req);
}

static esp_err_t logging_handler_static(httpd_req_t *req)
{
    auto *obj = static_cast<IApiLogging *>(req->user_ctx);
    return obj->logging_handler(req);
}

void ApiServer::register_generic_endpoints()
{
    httpd_uri_t status_uri = {
        .uri = "/api/v1/status",
        .method = HTTP_GET,
        .handler = status_handler_static,
        .user_ctx = static_cast<void *>(&apiStatus_)};
    espHttpServer_.httpd_register_uri_handler(server, &status_uri);

    httpd_uri_t status_stream_uri = {
        .uri = "/api/v1/status/stream",
        .method = HTTP_GET,
        .handler = status_stream_handler_static,
        .user_ctx = static_cast<void *>(&apiStatus_)};
    espHttpServer_.httpd_register_uri_handler(server, &status_stream_uri);

    httpd_uri_t nodes_info_uri = {
        .uri = "/api/v1/nodes_info",
        .method = HTTP_GET,
        .handler = nodes_info_handler_static,
        .user_ctx = static_cast<void *>(&apiNodes_)};
    espHttpServer_.httpd_register_uri_handler(server, &nodes_info_uri);

    httpd_uri_t save_nodes_info_uri = {
        .uri = "/api/v1/nodes_info",
        .method = HTTP_POST,
        .handler = save_nodes_info_handler_static,
        .user_ctx = static_cast<void *>(&apiNodes_)};
    espHttpServer_.httpd_register_uri_handler(server, &save_nodes_info_uri);

    httpd_uri_t reboot_uri = {
        .uri = "/api/v1/reboot",
        .method = HTTP_POST,
        .handler = reboot_handler_static,
        .user_ctx = static_cast<void *>(&apiSystem_)};
    espHttpServer_.httpd_register_uri_handler(server, &reboot_uri);

    httpd_uri_t factory_reset_uri = {
        .uri = "/api/v1/factory_reset",
        .method = HTTP_POST,
        .handler = factory_reset_handler_static,
        .user_ctx = static_cast<void *>(&apiSystem_)};
    espHttpServer_.httpd_register_uri_handler(server, &factory_reset_uri);

    httpd_uri_t firmware_chunk_uri = {
        .uri = "/api/v1/firmware_chunk/*",
        .method = HTTP_POST,
        .handler = firmware_chunk_handler_static,
        .user_ctx = static_cast<void *>(&apiFirmware_)};
    espHttpServer_.httpd_register_uri_handler(server, &firmware_chunk_uri);

    httpd_uri_t firmware_finish_uri = {
        .uri = "/api/v1/firmware_finish/*",
        .method = HTTP_POST,
        .handler = firmware_finish_handler_static,
        .user_ctx = static_cast<void *>(&apiFirmware_)};
    espHttpServer_.httpd_register_uri_handler(server, &firmware_finish_uri);

    httpd_uri_t esp_now_key_uri = {
        .uri = "/api/v1/esp_now_key",
        .method = HTTP_POST,
        .handler = esp_now_key_handler_static,
        .user_ctx = static_cast<void *>(&apiSecurity_)};
    espHttpServer_.httpd_register_uri_handler(server, &esp_now_key_uri);

    httpd_uri_t wifi_password_uri = {
        .uri = "/api/v1/wifi_password",
        .method = HTTP_POST,
        .handler = wifi_password_handler_static,
        .user_ctx = static_cast<void *>(&apiSecurity_)};
    espHttpServer_.httpd_register_uri_handler(server, &wifi_password_uri);

    httpd_uri_t logging_uri = {
        .uri = "/api/v1/logging",
        .method = HTTP_GET,
        .handler = logging_handler_static,
        .user_ctx = static_cast<void *>(&apiLogging_)};
    espHttpServer_.httpd_register_uri_handler(server, &logging_uri);
}

void ApiServer::register_static_file_handler()
{
    httpd_uri_t static_file_uri = {
        .uri = "/*",
        .method = HTTP_GET,
        .handler = ApiServer::static_file_handler_thunk,
        .user_ctx = this};
    espHttpServer_.httpd_register_uri_handler(server, &static_file_uri);
}

void ApiServer::mount_littlefs()
{
    esp_vfs_littlefs_conf_t conf = {
        .base_path = "/littlefs",
        .partition_label = "littlefs",
        .partition = NULL,
#if defined(CONFIG_LITTLEFS_SDMMC_SUPPORT)
        .sdcard = NULL,
#endif
        .format_if_mount_failed = false,
        .read_only = false,
        .dont_mount = false,
        .grow_on_mount = false};

    esp_err_t ret = espLittleFs_.esp_littlefs_mount(&conf);
    if (ret != ESP_OK)
    {
        if (ret == ESP_FAIL)
        {
            espLittleFs_.esp_littlefs_format("littlefs");
            ret = espLittleFs_.esp_littlefs_mount(&conf);
        }

        if (ret != ESP_OK)
        {
            printf("Error mounting LittleFS: %s\n", esp_err_to_name(ret));
        }
        else
        {
            printf("LittleFS mounted successfully\n");
        }
    }
    else
    {
        printf("LittleFS mounted successfully\n");
    }
}
