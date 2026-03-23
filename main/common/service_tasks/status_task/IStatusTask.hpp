#pragma once

/**
 * Interface for the status task.
 *
 * Provides periodic status updates pushed to clients via the
 * /api/v1/status SSE endpoint.
 */
class IStatusTask
{
public:
    virtual ~IStatusTask() = default;

    // TODO: define additional status task-specific interface methods
    //       (e.g., registerSseClient, updateNodeStatus, etc.)
};
