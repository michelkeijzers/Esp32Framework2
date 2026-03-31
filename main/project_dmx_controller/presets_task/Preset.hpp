#pragma once

#include <string>
#include <vector>
#include <cstdint>

/**
 * Value object for a DMX preset.
 *
 * Contains the preset metadata and 512 DMX channel values.
 */
struct Preset
{
    int         number;
    std::string name;
    bool        active;
    std::vector<uint8_t> dmx_values;

    Preset() = default;

    Preset(int number, std::string name, bool active)
        : number(number)
        , name(std::move(name))
        , active(active)
        , dmx_values(512, 0)
    {
    }
};
