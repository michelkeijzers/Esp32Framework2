#pragma once

/**
 * Interface for the logging task.
 *
 * Provides runtime log access to other parts of the system (e.g., streaming
 * log messages over the /api/v1/logging SSE endpoint).
 */
class ILoggingTask {
   public:
    virtual ~ILoggingTask() = default;

    // TODO: define additional logging task-specific interface methods
    //       (e.g., registerSseClient, setLogLevel, etc.)
};
