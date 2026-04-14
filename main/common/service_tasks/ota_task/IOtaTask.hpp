#pragma once

/**
 * Interface for the OTA (Over-The-Air firmware update) task.
 *
 * Manages firmware chunk reception and flashing for both the master
 * and slave nodes.
 */
class IOtaTask {
   public:
    virtual ~IOtaTask() = default;

    // TODO: define additional OTA task-specific interface methods
    //       (e.g., beginUpdate, writeChunk, finishUpdate, getProgress, etc.)
};
