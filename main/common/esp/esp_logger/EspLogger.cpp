#include "EspLogger.hpp"
#include "esp_logger_if.hpp"

void EspLogger::log_error(const char* tag, const char* message)
{
    ESP_LOGE(tag, "%s", message);
}

void EspLogger::log_info(const char* tag, const char* message)
{
    ESP_LOGI(tag, "%s", message);
}

void EspLogger::log_warn(const char* tag, const char* message)
{
    ESP_LOGW(tag, "%s", message);
}
