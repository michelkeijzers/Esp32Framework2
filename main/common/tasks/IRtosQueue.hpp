
#include <cstdint>

// Interface for RTOS-like queue (for DI)
class IRtosQueue {
public:
    virtual ~IRtosQueue() = default;
    virtual bool send(const void* item, uint32_t timeoutMs) = 0;
    virtual bool receive(void* item, uint32_t timeoutMs) = 0;
    virtual size_t itemSize() const = 0;
    virtual size_t capacity() const = 0;
    virtual size_t count() const = 0;
};