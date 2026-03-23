#pragma once

#include "Preset.hpp"
#include "../../common/esp/esp_error/esp_error_if.hpp"

#include <vector>
#include <cstdint>

/**
 * Interface for the preset manager.
 *
 * Manages DMX presets: in-memory CRUD operations plus persistence to NVS.
 */
class IPresetManager
{
public:
    virtual ~IPresetManager() = default;

    /**
     * Return all stored presets.
     */
    virtual std::vector<Preset> get_presets() = 0;

    /**
     * Return the preset numbers of all active presets.
     */
    virtual std::vector<int> get_active_presets() = 0;

    /**
     * Return a pointer to the preset with the given number, or nullptr if not found.
     *
     * @param number  Preset number.
     */
    virtual Preset *get_preset(uint32_t number) = 0;

    /**
     * Mark the specified preset as the selected (currently active) one.
     *
     * @param number  Preset number to select.
     * @return ESP_OK on success, ESP_ERR_NOT_FOUND if the preset does not exist.
     */
    virtual esp_err_t select_preset(uint32_t number) = 0;

    /**
     * Activate the blackout state (zero all DMX channels).
     * @return ESP_OK on success.
     */
    virtual esp_err_t blackout() = 0;

    /**
     * Save (create or update) a preset in the in-memory store.
     *
     * @param preset  Preset to save.
     * @return ESP_OK on success.
     */
    virtual esp_err_t save_preset(const Preset &preset) = 0;

    /**
     * Delete the preset with the given number.
     *
     * @param number  Preset number to delete.
     * @return ESP_OK on success, ESP_ERR_NOT_FOUND if the preset does not exist.
     */
    virtual esp_err_t delete_preset(uint32_t number) = 0;

    /**
     * Move a preset up or down in the ordered list.
     *
     * @param number  Preset number to move.
     * @param is_up   true = move up (lower index), false = move down (higher index).
     * @return ESP_OK on success.
     */
    virtual esp_err_t move_preset(uint32_t number, bool is_up) = 0;

    /**
     * Toggle the active flag of the specified preset.
     *
     * @param number  Preset number.
     * @return ESP_OK on success.
     */
    virtual esp_err_t swap_activation(uint32_t number) = 0;

    /**
     * Persist the current in-memory preset list to NVS.
     * @return ESP_OK on success.
     */
    virtual esp_err_t commit() = 0;
};
