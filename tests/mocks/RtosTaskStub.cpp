#include "tasks/RtosTask.hpp"
#include "tasks/RtosQueue.hpp"

// Stub implementation for test builds
RtosTask::RtosTask(const char *name, uint32_t stackSize, UBaseType_t priority)
    : name_(name), stackSize_(stackSize), priority_(priority) {}

esp_err_t RtosTask::init() { return 0; }
esp_err_t RtosTask::start() { return 0; }
IRtosQueue* RtosTask::createQueue(size_t, size_t) { return nullptr; }
