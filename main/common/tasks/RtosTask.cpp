#include "RtosTask.hpp"

RtosTask::RtosTask(const char *name, uint32_t stackSize, UBaseType_t priority)
    : name_(name), stackSize_(stackSize), priority_(priority)
{
}
