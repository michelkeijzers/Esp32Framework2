#pragma once

#include <cstddef>
#include <initializer_list>

namespace test_doubles
{
    struct State
    {
        int master_node_init_calls = 0;
        int master_node_start_calls = 0;
        int master_task_ctor_calls = 0;
        int web_server_task_ctor_calls = 0;
        int service_tasks_ctor_calls = 0;
    };

    inline State state;

    inline void reset_state()
    {
        state = State{};
    }
} // namespace test_doubles

class IEspNow
{
public:
    virtual ~IEspNow() = default;
};

class IFreeRtosFactory
{
public:
    virtual ~IFreeRtosFactory() = default;
};

class ICommonApiFactory
{
public:
    virtual ~ICommonApiFactory() = default;
};

class IEspFactory
{
public:
    virtual ~IEspFactory() = default;
    virtual IFreeRtosFactory &getFreeRtosFactory() = 0;
};

class IContextFactory
{
public:
    virtual ~IContextFactory() = default;
    virtual IEspFactory &getEspFactory() = 0;
    virtual ICommonApiFactory &getCommonApiFactory() = 0;
};

class StubFreeRtosFactory final : public IFreeRtosFactory
{
};

class StubCommonApiFactory final : public ICommonApiFactory
{
};

class StubEspFactory final : public IEspFactory
{
public:
    IFreeRtosFactory &getFreeRtosFactory() override
    {
        return freeRtosFactory;
    }

private:
    StubFreeRtosFactory freeRtosFactory;
};

class StubContextFactory final : public IContextFactory
{
public:
    IEspFactory &getEspFactory() override
    {
        return espFactory;
    }

    ICommonApiFactory &getCommonApiFactory() override
    {
        return commonApiFactory;
    }

private:
    StubEspFactory espFactory;
    StubCommonApiFactory commonApiFactory;
};

class StubEspNow final : public IEspNow
{
};

class DmxContextFactory
{
public:
    IContextFactory &getContextFactory()
    {
        return contextFactory;
    }

private:
    StubContextFactory contextFactory;
};

class EspNowFactory
{
public:
    IEspNow &getEspNow()
    {
        return espNow;
    }

private:
    StubEspNow espNow;
};

class MasterBridge
{
public:
    explicit MasterBridge(IEspNow &)
    {
    }
};

class RtosQueue
{
public:
    RtosQueue(std::size_t, std::size_t)
    {
    }
};

class ApiServer
{
public:
    ApiServer(IEspFactory &, ICommonApiFactory &)
    {
    }
};

class WebServerTask
{
public:
    WebServerTask(IFreeRtosFactory &, ApiServer &, const char *, std::size_t, int)
    {
        ++test_doubles::state.web_server_task_ctor_calls;
    }
};

class RtosQueueSet
{
public:
    explicit RtosQueueSet(std::size_t)
    {
    }
};

class MasterTask
{
public:
    MasterTask(IFreeRtosFactory &, MasterBridge &, RtosQueue &, WebServerTask &, RtosQueue &, RtosQueueSet &, const char *, std::size_t, int)
    {
        ++test_doubles::state.master_task_ctor_calls;
    }
};

class ServiceTasks
{
public:
    explicit ServiceTasks(IFreeRtosFactory &)
    {
        ++test_doubles::state.service_tasks_ctor_calls;
    }
};

class MasterNode
{
public:
    template <typename T1, typename T2>
    MasterNode(IFreeRtosFactory &, MasterBridge &, MasterTask &, WebServerTask &, ServiceTasks &, T1, T2)
    {
    }

    MasterNode(IFreeRtosFactory &, MasterBridge &, MasterTask &, WebServerTask &, ServiceTasks &, std::initializer_list<int>, std::initializer_list<int>)
    {
    }

    void init()
    {
        ++test_doubles::state.master_node_init_calls;
    }

    void start()
    {
        ++test_doubles::state.master_node_start_calls;
    }
};
