#include <cstring>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "RtosQueue.hpp"

RtosQueue::RtosQueue(size_t itemSize, size_t length)
    : m_itemSize(itemSize), m_length(length), m_queueHandle(nullptr)
{}

RtosQueue::~RtosQueue() {
    if (m_queueHandle) {
        vQueueDelete(static_cast<QueueHandle_t>(m_queueHandle));
        m_queueHandle = nullptr;
    }
}

bool RtosQueue::Create()
{
    m_queueHandle = xQueueCreate(m_length, m_itemSize);
    return m_queueHandle != nullptr;
}

bool RtosQueue::send(const void* item, uint32_t timeoutMs) {
    if (!m_queueHandle) return false;
    BaseType_t res = xQueueSend(static_cast<QueueHandle_t>(m_queueHandle), item, pdMS_TO_TICKS(timeoutMs));
    return res == pdPASS;
}

bool RtosQueue::receive(void* item, uint32_t timeoutMs) {
    if (!m_queueHandle) return false;
    BaseType_t res = xQueueReceive(static_cast<QueueHandle_t>(m_queueHandle), item, pdMS_TO_TICKS(timeoutMs));
    return res == pdPASS;
}

size_t RtosQueue::itemSize() const { return m_itemSize; }

size_t RtosQueue::capacity() const { return m_length; }

size_t RtosQueue::count() const {
    if (!m_queueHandle) return 0;
    return static_cast<size_t>(uxQueueMessagesWaiting(static_cast<QueueHandle_t>(m_queueHandle)));
}
