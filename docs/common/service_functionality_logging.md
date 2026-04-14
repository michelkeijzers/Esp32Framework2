# SERVICE FUNCTIONALITY: LOGGING

# OVERVIEW

Logging needs to be handled in various ways, such as sending it via UART and sending it to the website.
Logging can be initiated from any task. To have logging on the website it needs to be send to the master node first if its initiated from a slave node.

Four types of logging items need to be supported:

- Error
- Warning
- Info
- Debug

Each logging item also has a timestamp and for debug a log level.
Per task, for debug the log level can be set, which means that only logs with a log level equal or higher than the set log level will be logged.

The logging will be stored in a file. When the file reaches a certain size a new file will be created. Upto a certain amount of files will be kept.

# SCENARIO 1: LOGGING SETTINGS

## Overview

On the website, the user can set the log levels for each task.
The system will only send log messages to the webserver if the message will be displayed.

## Requirements

- FR-LOG-001: The system shall allow the user to set the log level for debug messages for each task.
- FR-LOG-002: The system shall allow the user to enable/disable logging of error, warning, and info messages for each task.
- FR-LOG-003: The system shall only send log messages to the webserver if they will be displayed based on the set log levels and enabled log types.
- FR-LOG-004: The system shall send log messages to the webserver with the following information: Node ID, Task ID, Timestamp, Log Type (Error, Warning, Info, Debug), Log Level (for debug), and Log Message.
- FR-LOG-005: The system shall store log messages in a file and manage the log files based on size and retention policies.
- FR-LOG-006: The system shall allow the user to view log messages on the website in a readable format.
- FR-LOG-007: The system shall allow the user to filter log messages on the website based on Node ID, Task ID, Log Type, and Log Level. REJECTED
- FR-LOG-008: The system shall allow the user to clear log messages from the website.
- FR-LOG-009: The system shall allow the user to download log messages from the website.

## Website

- Button Settings
- Logging, see Scenario 3.2 for details

When the Settings button is clicked, the following screen is shown with a list of items.

Node ID Node Name Task ID Task Name Error Warning Info Debug Log Level  
12 Master 1 Master Task Check Check Check Check 5
...

Whenever a a Check/Uncheck is clicked, a message is sent.

## Sequence Diagram

```mermaid
sequenceDiagram
    box green Website
        participant Website
    end
    box blue Master
        participant MasterWebserverTask
        participant MasterTask
        participant MasterDestinationTask
    end

    Website->>MasterWebserverTask: GET /api/v1/logging/log_level

    TODO: Gather all, add to API contract

    Website->>MasterWebserverTask: POST /api/v1/logging/log_level
    MasterWebserverTask->>MasterTask: Set log level
    MasterTask->>MasterDestinationTask: Set log level
```

## Class Diagram

n.a.

## API Contract

### GET /api/v1/logging/log_level

Retrieves the logging enable state or log level for a log type for a task.
For `debug`, the log level (0-255) is retrieved. For `error`, `warning`, and `info`, only whether logging is enabled is relevant.

**Parameters:**

- uint8_t nodeId
- uint8_t taskId
- string type : "error", "warning", "info", "debug"

**Responses:**

- 200: OK
- 400: Bad Request: Invalid log type, missing parameters, or invalid parameter values
- 404: Not Found: Node or task not found

**Example request (debug):**

```json
{
  "nodeId": 12,
  "taskId": 1,
  "type": "debug"
}
```

Response:

Status: 200 OK

```json
{
  "enabled": true,
  "log_level": 5
}
```

**Example request (warning):**

```json
{
  "nodeId": 12,
  "taskId": 1,
  "type": "warning"
}
```

Response:

Status: 200 OK

```json
{
  "enabled": true
}
```

## POST /api/v1/logging/log_level

Sets the logging enable state or log level for a log type for a task.
For `debug`, set both `enabled` and `log_level` (0-255). For `error`, `warning`, and `info`, set only `enabled` (true/false).

**Parameters:**

- uint8_t nodeId
- uint8_t taskId
- string type : "error", "warning", "info", "debug"
- bool enabled
- uint8_t log_level (only for debug)

**Responses:**

- 200: OK
- 400: Bad Request: Invalid log type, missing parameters, or invalid parameter values, log level out of range, log level provided for non-debug type, or log level missing for debug type.
- 404: Not Found: Node or task not found

**Example request (set debug log level):**

```json
{
  "nodeId": 12,
  "taskId": 1,
  "type": "debug",
  "enabled": true,
  "log_level": 5
}
```

Response:

Status: 200 OK

```json
{}
```

**Example request (enable warning logging):**

```json
{
  "nodeId": 12,
  "taskId": 1,
  "type": "warning",
  "enabled": true
}
```

Response:

Status: 200 OK

```json
{}
```

## ESP-NOW Messages

This message is only used for slave nodes and therefore will be described later.

# SCENARIO 2: LOGGING MESSAGES

## Overview

Logging messages are only send if they will be displayed on the website. However, in all cases they are sent through UART or ESP-NOW.

Slave nodes will be described later.

## Requirements

## Website

- Button <Settings>, see Scenario 3.1 for details
- MultiTextBox: items of :
  Node <ID>:<Node Name>, Task <ID>:<Task Name>, [<Timestamp>] <Type> (<Log Level>): <Log Message

## Sequence Diagram

n.a.

## Class Diagram

n.a.

## API Contract

### GET /api/v1/logging/stream

Retrieves a stream of log messages that are sent to the webserver. The stream will only include log messages that are enabled and meet the log level criteria for display on the website.

**Parameters:**

None

**Responses:**

Each logging event results in a message like below.
Note: For debug messages, the log_level field is relevant and included. For other message types, the log_level field is not relevant and is omitted.

```json
{
  "timestamp": "2024-04-13T12:34:56Z",
  "nodeId": 12,
  "nodeName": "Master",
  "taskId": 1,
  "taskName": "Master Task",
  "type": "debug",
  "log_level": 5,
  "message": "System initialized"
}
```

## ESP-NOW Messages

This message is only used for slave nodes and therefore will be described later.

# FUTURE ADDITIONS/IMPROVEMENTS

- Logging from slave nodes, which includes sending the log messages to the master node via ESP-NOW and then to the webserver.
- Filtering messages on the website
- Storing messages from the website to a file.
