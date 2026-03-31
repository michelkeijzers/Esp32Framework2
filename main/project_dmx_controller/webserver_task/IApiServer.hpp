#pragma once

/**
 * Interface for project API server implementations.
 */
class IApiServer
{
public:
    virtual ~IApiServer() = default;

    /**
     * Start the HTTP server and register all API endpoints.
     */
    virtual void start() = 0;

    /**
     * Stop the HTTP server and deregister all endpoints.
     */
    virtual void stop() = 0;
};
