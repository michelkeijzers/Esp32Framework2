#include "ApiPresetValues.hpp"
#include "IEspHttpServer.hpp"
#include "../../../common/esp/esp_http_server/UriParamExtractor.hpp"

ApiPresetValues::ApiPresetValues(IEspHttpServer& espHttpServer, IPresetManager& presetManager) 
    : espHttpServer_(espHttpServer), presetManager_(presetManager) {}
ApiPresetValues::~ApiPresetValues() = default;

esp_err_t ApiPresetValues::get_preset_values_handler(httpd_req_t *req) {
    // Extract preset number from URI: /api/v1/preset_values/<num>
    uint32_t preset_num = UriParamExtractor::extractFirstParamAsUint32(
        "/api/v1/preset_values/*", req->uri);
    
    auto preset = presetManager_.get_preset(preset_num);
    if (!preset) {
        espHttpServer_.httpd_resp_set_type(req, "application/json");
        espHttpServer_.httpd_resp_send(req, "{\"error\":\"preset not found\"}", HTTPD_RESP_USE_STRLEN);
        return ESP_ERR_NOT_FOUND;
    }
    
    // Build JSON array of DMX values
    std::string json_resp = "[";
    for (size_t i = 0; i < preset->dmx_values.size(); ++i) {
        if (i > 0) json_resp += ",";
        json_resp += std::to_string(preset->dmx_values[i]);
    }
    json_resp += "]";
    
    espHttpServer_.httpd_resp_set_type(req, "application/json");
    espHttpServer_.httpd_resp_send(req, json_resp.c_str(), HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t ApiPresetValues::set_preset_value_handler(httpd_req_t *req) {
    // Extract preset number and channel from URI: /api/v1/preset_value/<preset>/<channel>
    // Value should ideally come from request body (JSON: {"value": 255})
    // For now, we extract it from the third wildcard if available
    auto params = UriParamExtractor::extractParams(
        "/api/v1/preset_value/*/*", req->uri);
    
    if (params.size() < 2) {
        espHttpServer_.httpd_resp_set_type(req, "application/json");
        espHttpServer_.httpd_resp_send(req, "{\"error\":\"missing parameters\"}", HTTPD_RESP_USE_STRLEN);
        return ESP_ERR_INVALID_ARG;
    }
    
    uint32_t preset_num = 0;
    uint32_t channel_idx = 0;
    uint8_t value = 0;
    
    // Safe string to uint32_t conversion without exceptions
    for (char c : params[0]) {
        if (c < '0' || c > '9') break;
        preset_num = preset_num * 10 + (c - '0');
    }
    
    for (char c : params[1]) {
        if (c < '0' || c > '9') break;
        channel_idx = channel_idx * 10 + (c - '0');
    }
    
    // If a third parameter exists (value), use it; otherwise default to 0
    if (params.size() > 2) {
        uint32_t val = 0;
        for (char c : params[2]) {
            if (c < '0' || c > '9') break;
            val = val * 10 + (c - '0');
        }
        value = static_cast<uint8_t>(val & 0xFF);
    }
    
    auto preset = presetManager_.get_preset(preset_num);
    if (!preset) {
        espHttpServer_.httpd_resp_set_type(req, "application/json");
        espHttpServer_.httpd_resp_send(req, "{\"error\":\"preset not found\"}", HTTPD_RESP_USE_STRLEN);
        return ESP_ERR_NOT_FOUND;
    }
    
    if (channel_idx >= 512) {
        espHttpServer_.httpd_resp_set_type(req, "application/json");
        espHttpServer_.httpd_resp_send(req, "{\"error\":\"channel index out of range\"}", HTTPD_RESP_USE_STRLEN);
        return ESP_ERR_INVALID_ARG;
    }
    
    // Update DMX value (modify in-place, then commit)
    Preset updated_preset = *preset;
    updated_preset.dmx_values[channel_idx] = value;
    
    esp_err_t err = presetManager_.save_preset(updated_preset);
    if (err != ESP_OK) {
        espHttpServer_.httpd_resp_set_type(req, "application/json");
        espHttpServer_.httpd_resp_send(req, "{\"error\":\"failed to save preset\"}", HTTPD_RESP_USE_STRLEN);
        return err;
    }
    
    err = presetManager_.commit();
    if (err != ESP_OK) {
        espHttpServer_.httpd_resp_set_type(req, "application/json");
        espHttpServer_.httpd_resp_send(req, "{\"error\":\"failed to commit\"}", HTTPD_RESP_USE_STRLEN);
        return err;
    }
    
    std::string resp = "{\"index\":" + std::to_string(channel_idx) + ",\"value\":" + std::to_string(value) + "}";
    espHttpServer_.httpd_resp_set_type(req, "application/json");
    espHttpServer_.httpd_resp_send(req, resp.c_str(), HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}
