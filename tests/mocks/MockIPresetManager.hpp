#pragma once
#include <gmock/gmock.h>

#include "IPresetManager.hpp"

class MockIPresetManager : public IPresetManager {
   public:
    MOCK_METHOD(std::vector<Preset>, get_presets, (), (override));
    MOCK_METHOD(std::vector<int>, get_active_presets, (), (override));
    MOCK_METHOD(Preset *, get_preset, (uint32_t), (override));
    MOCK_METHOD(esp_err_t, select_preset, (uint32_t), (override));
    MOCK_METHOD(esp_err_t, blackout, (), (override));
    MOCK_METHOD(esp_err_t, save_preset, (const Preset &), (override));
    MOCK_METHOD(esp_err_t, delete_preset, (uint32_t), (override));
    MOCK_METHOD(esp_err_t, move_preset, (uint32_t, bool), (override));
    MOCK_METHOD(esp_err_t, swap_activation, (uint32_t), (override));
    MOCK_METHOD(esp_err_t, commit, (), (override));
};
