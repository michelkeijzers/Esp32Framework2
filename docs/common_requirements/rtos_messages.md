# RTOS MESSAGES BETWEEN SERVICE TASKS

# Introduction

This document outlines the messages exchanged between various service tasks in the RTOS environment. Each message is defined with its source task, destination task, and the data it carries. This communication is crucial for maintaining the functionality and coordination of tasks such as monitoring, logging, and over-the-air updates.

It is irrelevant for the messages if the message is sent directly between tasks, via the Master Task (which is default) or via ESP-NOW in case the task is residing on a slave node. The Master Task will forward the message to the destination task regardless of the source.

# Status Task

| Message           | Source      | Destination     | Field     | Data Type | Description                                                                                                                                                             |
| ----------------- | ----------- | --------------- | --------- | --------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| SET_MONITOR_TASK  | Master Task | Status Task     | task_id   | int       | Let status task know which task to check the heartbeat for. This message will be sent for each task to monitor including the Master Task itself.                        |
|                   |             |                 | task_name | string    | Task name.                                                                                                                                                              |
| HEARTBEAT         | Any Task    | Status Task     | task_id   | int       | Sent by a task to indicate that the task is still alive. This message should be sent approximately every 3 seconds.                                                     |
| HEARTBEAT_TIMEOUT | Status Task | Web Server Task | task_id   | int       | Task name that has not sent a heartbeat within the expected time frame. This message is sent if the heartbeat of a task is not received within approximately 5 seconds. |
|                   |             |                 | task_name | string    | Task name.                                                                                                                                                              |

# Logging Task

| Message       | Source          | Destination  | Field     | Data Type | Description                                   |
| ------------- | --------------- | ------------ | --------- | --------- | --------------------------------------------- |
| SET_LOG_LEVEL | Web Server Task | Logging Task | category  | uint8     | Log category (e.g., INFO, WARNING, ERROR).    |
|               |                 |              | log_level | uint8     | Set the log level for the specified category. |
| LOG_MESSAGE   | Any Task        | Logging Task | category  | uint8     | Log category (e.g., INFO, WARNING, ERROR).    |
|               |                 |              | log_level | uint8     | Log level for the specified category.         |
|               |                 |              | message   | string    | String to be logged.                          |

# OTA Task

| Message               | Source          | Destination     | Field              | Data Type   | Description                                                                                                                                                                                                                     |
| --------------------- | --------------- | --------------- | ------------------ | ----------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| OTA_UPDATE_REQUEST    | Web Server Task | OTA Task        | node_id            | uint8       | Node ID of the device to receive the OTA update. TODO: node_id or MAC address?                                                                                                                                                  |
|                       |                 |                 | version            | string      | Version of the OTA update.                                                                                                                                                                                                      |
|                       |                 |                 | number_of_packages | uint16      | Number of packages that the web server task will sent (4 bytes).                                                                                                                                                                |
| OTA_UPDATE_REQUEST_OK | OTA Task        | Web Server Task | -                  | -           | If not received, the web server task will show an error that OTA will not start due to an unresponsive OTA task.                                                                                                                |
| OTA_PACKAGE           | Web Server Task | OTA Task        | package_number     | uint16      | Package number. The next message is only sent after a OTA_PACKAGE_RESULT                                                                                                                                                        |
|                       |                 |                 | data               | uint8[1024] | OTA package data, 1 KB of data per package.                                                                                                                                                                                     |
|                       |                 |                 | checksum           | uint32      | Checksum of the package data.                                                                                                                                                                                                   |
| OTA_PACKAGE_RESULT    | OTA Task        | Web Server Task | package_number     | uint16      | Package number. The next package will only be sent after receiving this message.                                                                                                                                                |
|                       |                 |                 | success            | bool        | Indicates whether the package was received successfully. A resent will be done if success is false, if the package number is not equal to the expected package number, or if the result has not been received within 3 seconds. |
| OTA_UPDATE_COMPLETE   | OTA Task        | Web Server Task | -                  | -           | Indicates that the OTA update process is complete. A restart will follow. If not received within 3 seconds after the last OTA_PACKAGE_RESULT, a message on the web server will be shown.                                        |
