#pragma once

#include "IPresetManager.hpp"

#include <vector>
#include <cstdint>

/**
 * Concrete preset manager with in-memory storage.
 *
 * Loads preset data from NVS on init and persists changes via commit().
 */
class PresetManager : public IPresetManager
{
public:
    PresetManager();
    ~PresetManager() override = default;

    std::vector<Preset> get_presets()                override;
    std::vector<int>    get_active_presets()          override;
    Preset             *get_preset(uint32_t number)   override;
    esp_err_t           select_preset(uint32_t number) override;
    esp_err_t           blackout()                    override;
    esp_err_t           save_preset(const Preset &preset) override;
    esp_err_t           delete_preset(uint32_t number) override;
    esp_err_t           move_preset(uint32_t number, bool is_up) override;
    esp_err_t           swap_activation(uint32_t number) override;
    esp_err_t           commit()                      override;

private:
    std::vector<Preset> presets_;
    int                 selectedPresetNumber_;

    /** Return the index in presets_ for the given number, or -1 if not found. */
    int findIndex(uint32_t number) const;
};
