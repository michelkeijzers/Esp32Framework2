#include "SubTask.hpp"

SubTask::SubTask(const char *name, uint32_t stackSize, UBaseType_t priority)
    : RtosTask(name, stackSize, priority)
{
}
