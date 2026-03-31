// Stub implementation of RtosQueue for test builds.
// No FreeRTOS dependency — safe to link on host (Windows/Linux).
#include "common/tasks/RtosQueue.hpp"

RtosQueue::RtosQueue(size_t itemSize, size_t length)
    : m_itemSize(itemSize), m_length(length), m_queueHandle(nullptr) {}

bool RtosQueue::create() { return true; }

QueueHandle_t RtosQueue::getHandle() { return m_queueHandle; }

RtosQueue::~RtosQueue() {}

bool RtosQueue::send(const void*, uint32_t) { return false; }

bool RtosQueue::receive(void*, uint32_t) { return false; }

size_t RtosQueue::itemSize() const { return m_itemSize; }

size_t RtosQueue::capacity() const { return m_length; }

size_t RtosQueue::count() const { return 0; }
