
#include "ApiPresets.hpp"

#include <sstream>

#include "../../../common/esp/esp_http_server/UriParamExtractor.hpp"
#include "IEspHttpServer.hpp"

ApiPresets::ApiPresets(IEspHttpServer &espHttpServer, IPresetManager &presetManager)
    : espHttpServer_(espHttpServer), presetManager_(presetManager) {}
ApiPresets::~ApiPresets() = default;

esp_err_t ApiPresets::get_presets_handler(httpd_req_t *req) {
    auto presets = presetManager_.get_presets();

    std::string json_resp = "[";
    for (size_t i = 0; i < presets.size(); ++i) {
        if (i > 0) json_resp += ",";
        json_resp += "{\"number\":" + std::to_string(presets[i].number) + ",";
        json_resp += "\"name\":\"" + presets[i].name + "\",";
        json_resp += "\"active\":" + std::string(presets[i].active ? "true" : "false") + "}";
    }
    json_resp += "]";

    espHttpServer_.httpd_resp_set_type(req, "application/json");
    espHttpServer_.httpd_resp_send(req, json_resp.c_str(), HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t ApiPresets::save_preset_handler(httpd_req_t *req) {
    // Note: In a full implementation, parse request body for preset data
    // For now, assume data is provided via URI parameter or body
    espHttpServer_.httpd_resp_set_type(req, "application/json");
    esp_err_t err = presetManager_.commit();
    if (err != ESP_OK) {
        espHttpServer_.httpd_resp_send(req, "{\"ack\":\"nok\"}", HTTPD_RESP_USE_STRLEN);
        return err;
    }
    espHttpServer_.httpd_resp_send(req, "{\"ack\":\"ok\"}", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t ApiPresets::get_active_preset_numbers_handler(httpd_req_t *req) {
    auto active = presetManager_.get_active_presets();

    std::string json_resp = "[";
    for (size_t i = 0; i < active.size(); ++i) {
        if (i > 0) json_resp += ",";
        json_resp += std::to_string(active[i]);
    }
    json_resp += "]";

    espHttpServer_.httpd_resp_set_type(req, "application/json");
    espHttpServer_.httpd_resp_send(req, json_resp.c_str(), HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t ApiPresets::select_preset_handler(httpd_req_t *req) {
    // Extract preset number from URI: /api/v1/select_preset/<num>
    uint32_t preset_num =
        UriParamExtractor::extractFirstParamAsUint32("/api/v1/select_preset/*", req->uri);

    esp_err_t err = presetManager_.select_preset(preset_num);
    if (err != ESP_OK) {
        espHttpServer_.httpd_resp_set_type(req, "application/json");
        espHttpServer_.httpd_resp_send(req, "{\"ack\":\"nok\"}", HTTPD_RESP_USE_STRLEN);
        return err;
    }

    err = presetManager_.commit();
    if (err != ESP_OK) {
        espHttpServer_.httpd_resp_set_type(req, "application/json");
        espHttpServer_.httpd_resp_send(req, "{\"ack\":\"nok\"}", HTTPD_RESP_USE_STRLEN);
        return err;
    }

    auto preset = presetManager_.get_preset(preset_num);
    std::string resp = "{\"preset_name\":\"" + preset->name + "\"}";

    espHttpServer_.httpd_resp_set_type(req, "application/json");
    espHttpServer_.httpd_resp_send(req, resp.c_str(), HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t ApiPresets::blackout_handler(httpd_req_t *req) {
    esp_err_t err = presetManager_.blackout();
    if (err != ESP_OK) {
        espHttpServer_.httpd_resp_set_type(req, "application/json");
        espHttpServer_.httpd_resp_send(req, "{\"ack\":\"nok\"}", HTTPD_RESP_USE_STRLEN);
        return err;
    }

    err = presetManager_.commit();
    if (err != ESP_OK) {
        espHttpServer_.httpd_resp_set_type(req, "application/json");
        espHttpServer_.httpd_resp_send(req, "{\"ack\":\"nok\"}", HTTPD_RESP_USE_STRLEN);
        return err;
    }

    espHttpServer_.httpd_resp_set_type(req, "application/json");
    espHttpServer_.httpd_resp_send(req, "{\"preset_name\":\"Blackout\"}", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t ApiPresets::move_preset_up_handler(httpd_req_t *req) {
    // Extract preset number from URI: /api/v1/presets/<num>/move_up
    uint32_t preset_num =
        UriParamExtractor::extractFirstParamAsUint32("/api/v1/presets/*/move_up", req->uri);

    esp_err_t err = presetManager_.move_preset(preset_num, true);
    if (err != ESP_OK) {
        espHttpServer_.httpd_resp_set_type(req, "application/json");
        espHttpServer_.httpd_resp_send(req, "{\"ack\":\"nok\"}", HTTPD_RESP_USE_STRLEN);
        return err;
    }

    err = presetManager_.commit();
    if (err != ESP_OK) {
        espHttpServer_.httpd_resp_set_type(req, "application/json");
        espHttpServer_.httpd_resp_send(req, "{\"ack\":\"nok\"}", HTTPD_RESP_USE_STRLEN);
        return err;
    }

    // Return updated preset list
    return get_presets_handler(req);
}

esp_err_t ApiPresets::move_preset_down_handler(httpd_req_t *req) {
    // Extract preset number from URI: /api/v1/presets/<num>/move_down
    uint32_t preset_num =
        UriParamExtractor::extractFirstParamAsUint32("/api/v1/presets/*/move_down", req->uri);

    esp_err_t err = presetManager_.move_preset(preset_num, false);
    if (err != ESP_OK) {
        espHttpServer_.httpd_resp_set_type(req, "application/json");
        espHttpServer_.httpd_resp_send(req, "{\"ack\":\"nok\"}", HTTPD_RESP_USE_STRLEN);
        return err;
    }

    err = presetManager_.commit();
    if (err != ESP_OK) {
        espHttpServer_.httpd_resp_set_type(req, "application/json");
        espHttpServer_.httpd_resp_send(req, "{\"ack\":\"nok\"}", HTTPD_RESP_USE_STRLEN);
        return err;
    }

    // Return updated preset list
    return get_presets_handler(req);
}

esp_err_t ApiPresets::delete_preset_handler(httpd_req_t *req) {
    // Extract preset number from URI: /api/v1/presets/<num>
    uint32_t preset_num =
        UriParamExtractor::extractFirstParamAsUint32("/api/v1/presets/*", req->uri);

    esp_err_t err = presetManager_.delete_preset(preset_num);
    if (err != ESP_OK) {
        espHttpServer_.httpd_resp_set_type(req, "application/json");
        espHttpServer_.httpd_resp_send(req, "{\"ack\":\"nok\"}", HTTPD_RESP_USE_STRLEN);
        return err;
    }

    err = presetManager_.commit();
    if (err != ESP_OK) {
        espHttpServer_.httpd_resp_set_type(req, "application/json");
        espHttpServer_.httpd_resp_send(req, "{\"ack\":\"nok\"}", HTTPD_RESP_USE_STRLEN);
        return err;
    }

    // Return updated preset list
    return get_presets_handler(req);
}

esp_err_t ApiPresets::insert_preset_at_handler(httpd_req_t *req) {
    // Extract position from URI: /api/v1/presets/<num>/insert_at
    uint32_t position =
        UriParamExtractor::extractFirstParamAsUint32("/api/v1/presets/*/insert_at", req->uri);
    Preset new_preset(position, "New Preset", false);

    esp_err_t err = presetManager_.save_preset(new_preset);
    if (err != ESP_OK) {
        espHttpServer_.httpd_resp_set_type(req, "application/json");
        espHttpServer_.httpd_resp_send(req, "{\"ack\":\"nok\"}", HTTPD_RESP_USE_STRLEN);
        return err;
    }

    err = presetManager_.commit();
    if (err != ESP_OK) {
        espHttpServer_.httpd_resp_set_type(req, "application/json");
        espHttpServer_.httpd_resp_send(req, "{\"ack\":\"nok\"}", HTTPD_RESP_USE_STRLEN);
        return err;
    }

    // Return updated preset list
    return get_presets_handler(req);
}

esp_err_t ApiPresets::swap_preset_activation_handler(httpd_req_t *req) {
    // Extract preset number from URI: /api/v1/presets/<num>/swap_activation
    uint32_t preset_num =
        UriParamExtractor::extractFirstParamAsUint32("/api/v1/presets/*/swap_activation", req->uri);

    esp_err_t err = presetManager_.swap_activation(preset_num);
    if (err != ESP_OK) {
        espHttpServer_.httpd_resp_set_type(req, "application/json");
        espHttpServer_.httpd_resp_send(req, "{\"ack\":\"nok\"}", HTTPD_RESP_USE_STRLEN);
        return err;
    }

    err = presetManager_.commit();
    if (err != ESP_OK) {
        espHttpServer_.httpd_resp_set_type(req, "application/json");
        espHttpServer_.httpd_resp_send(req, "{\"ack\":\"nok\"}", HTTPD_RESP_USE_STRLEN);
        return err;
    }

    // Return updated preset list
    return get_presets_handler(req);
}
