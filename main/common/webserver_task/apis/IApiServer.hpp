#pragma once

/**
 * Interface for API Server for DI and testing
 */
class IApiServer {
public:
    virtual ~IApiServer() = default;
    virtual void start() = 0;
    virtual void stop() = 0;
};
