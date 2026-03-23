#pragma once

/**
 * Interface for all API server implementations.
 *
 * An API server registers HTTP handlers on the underlying HTTP server and
 * exposes the application REST API.
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
