#pragma once

#include "../../../common/sub_tasks/ISubTask.hpp"

#include <cstdint>

/**
 * Interface for the DMX MAX3485 sender sub task.
 *
 * Transmits DMX512 frames over a RS-485 bus via a MAX3485 transceiver.
 */
class IDmxMax3485SenderTask : public ISubTask
{
public:
    virtual ~IDmxMax3485SenderTask() = default;

    /**
     * Update the DMX value for a specific channel.
     *
     * @param channel  DMX channel index (0-511).
     * @param value    Channel value (0-255).
     */
    virtual void setChannelValue(uint16_t channel, uint8_t value) = 0;

    /**
     * Update all 512 DMX channel values at once.
     *
     * @param values  Array of 512 byte values (index = channel).
     */
    virtual void setAllChannelValues(const uint8_t values[512]) = 0;
};
