#include "PresetManager.hpp"

#include <algorithm>
#include <cstring>

PresetManager::PresetManager()
    : selectedPresetNumber_(-1)
{
    // TODO: load presets from NVS on construction
}

int PresetManager::findIndex(uint32_t number) const
{
    for (int i = 0; i < static_cast<int>(presets_.size()); ++i)
    {
        if (static_cast<uint32_t>(presets_[i].number) == number)
        {
            return i;
        }
    }
    return -1;
}

std::vector<Preset> PresetManager::get_presets()
{
    return presets_;
}

std::vector<int> PresetManager::get_active_presets()
{
    std::vector<int> result;
    for (const auto &p : presets_)
    {
        if (p.active)
        {
            result.push_back(p.number);
        }
    }
    return result;
}

Preset *PresetManager::get_preset(uint32_t number)
{
    int idx = findIndex(number);
    if (idx < 0) return nullptr;
    return &presets_[idx];
}

esp_err_t PresetManager::select_preset(uint32_t number)
{
    int idx = findIndex(number);
    if (idx < 0) return ESP_ERR_NOT_FOUND;
    selectedPresetNumber_ = static_cast<int>(number);
    return ESP_OK;
}

esp_err_t PresetManager::blackout()
{
    // TODO: drive DMX output to zero via DmxMax3485SenderTask
    selectedPresetNumber_ = -1;
    return ESP_OK;
}

esp_err_t PresetManager::save_preset(const Preset &preset)
{
    int idx = findIndex(static_cast<uint32_t>(preset.number));
    if (idx >= 0)
    {
        presets_[idx] = preset;
    }
    else
    {
        presets_.push_back(preset);
    }
    return ESP_OK;
}

esp_err_t PresetManager::delete_preset(uint32_t number)
{
    int idx = findIndex(number);
    if (idx < 0) return ESP_ERR_NOT_FOUND;
    presets_.erase(presets_.begin() + idx);
    return ESP_OK;
}

esp_err_t PresetManager::move_preset(uint32_t number, bool is_up)
{
    int idx = findIndex(number);
    if (idx < 0) return ESP_ERR_NOT_FOUND;

    if (is_up && idx > 0)
    {
        std::swap(presets_[idx], presets_[idx - 1]);
    }
    else if (!is_up && idx < static_cast<int>(presets_.size()) - 1)
    {
        std::swap(presets_[idx], presets_[idx + 1]);
    }
    return ESP_OK;
}

esp_err_t PresetManager::swap_activation(uint32_t number)
{
    int idx = findIndex(number);
    if (idx < 0) return ESP_ERR_NOT_FOUND;
    presets_[idx].active = !presets_[idx].active;
    return ESP_OK;
}

esp_err_t PresetManager::commit()
{
    // TODO: serialise presets_ and persist to NVS
    return ESP_OK;
}
