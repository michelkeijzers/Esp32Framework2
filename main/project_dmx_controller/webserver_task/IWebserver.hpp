#pragma once

/**
 * Interface for Webserver for DI and testing
 */
class IWebserver {
public:
    virtual ~IWebserver() = default;
    virtual void start() = 0;
    virtual void stop() = 0;
};
