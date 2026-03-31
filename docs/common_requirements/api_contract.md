# Common API Contract

This document describes the API contract between the frontend (web UI) and the backend (Flask server or ESP32 web server) for any project.

# Table of Contents

- [Status Page](#status-page)
  - [Get Node Status](#get-node-status)
  - [Node Status Stream (SSE)](#node-status-stream-sse)
- [Nodes Page](#nodes-page)
  - [Opening page](#opening-page)
  - [Save Button](#save-button-1)
  - [Reboot Button](#reboot-button)
  - [Factory Reset](#factory-reset)
  - [Firmware Update](#firmware-update)
- [Security Page](#security-page)
  - [Send ESP-NOW Security Key](#send-esp-now-security-key)
  - [Send Wi-Fi Password](#send-wi-fi-password)
- [Logging Page](#logging-page)
  - [Logging Stream (SSE)](#logging-stream-sse)

# Status Page

## Get Node Status

**Endpoint:** `GET /api/v1/status`

**Description:** Returns an array of all ESP32 nodes with their current status and metadata. Used to display the status dashboard in the web UI.

**Returns:** `200 OK` with JSON array of node status objects.

**Response Specification:**
Array of node objects, each containing:

- `name` (string, required) – Node identifier (e.g., "Master", "Node")
- `role` (string, required) – Role of the node (e.g., "Master", "Node")
- `node_type` (string, required) – Type of node (e.g., "WebServer", "DMX", "MIDI")
- `node_sequence` (integer, required) – Sequence number of the node (1-based)
- `status_watchdog` (string, required) – Watchdog status (e.g., "OK", "ERROR", "TIMEOUT")
- `last_communication` (string, required) – ISO 8601 timestamp of last communication
- `uptime` (string, required) – Human-readable uptime (e.g., "3h 12m")
- `firmware_version` (string, required) – Version string (e.g., "v1.2.3")
- `config_version` (string, required) – Configuration version (e.g., "cfg-2026-03-10")
- `mac_address` (string, required) – MAC address in format XX:XX:XX:XX:XX:XX
- `ip_address` (string, required) – IPv4 address (e.g., "192.168.1.101")

## Node Status Stream (SSE)

**Endpoint:** `GET /api/v1/status/stream`

**Description:** Streams real-time node status updates using Server-Sent Events (SSE). Each event contains the latest array of node status objects. Used for live updates on the Status page.

**Returns:** `200 OK` with `Content-Type: text/event-stream` and a stream of JSON arrays. Connection remains open while the page is active.

**Response Specification:**
Server-sent events (one per update) containing an array of node objects (same structure as Get Node Status endpoint). Each event includes:

- `event` (string) – Event type identifier ("status")
- `data` (JSON array) – Array of node status objects

Example request:

```
GET /api/v1/status/stream
```

Example event:

```
event: status
data: [{ "name": "Master", "role": "Master", "node_type": "WebServer", "node_sequence": 1, "status_watchdog": "OK", "last_communication": "2026-03-11 14:23:01", "uptime": "3h 12m", "firmware_version": "v1.2.3", "config_version": "cfg-2026-03-10", "mac_address": "24:6F:28:AA:BB:CC", "ip_address": "192.168.1.101" }, { "name": "Node", "role": "Node", "node_type": "DMX", "node_sequence": 2, "status_watchdog": "ERROR", "last_communication": "2026-03-11 14:25:10", "uptime": "2h 45m", "firmware_version": "v1.2.2", "config_version": "cfg-2026-03-09", "mac_address": "24:6F:28:DD:EE:FF", "ip_address": "192.168.1.102" }]
```

Example response (JSON):

```json
[
  {
    "name": "Master",
    "role": "Master",
    "node_type": "WebServer",
    "node_sequence": 1,
    "status_watchdog": "OK",
    "last_communication": "2026-03-11 14:23:01",
    "uptime": "3h 12m",
    "firmware_version": "v1.2.3",
    "config_version": "cfg-2026-03-10",
    "mac_address": "24:6F:28:AA:BB:CC",
    "ip_address": "192.168.1.101"
  },
  {
    "name": "Node",
    "role": "Node",
    "node_type": "DMX",
    "node_sequence": 2,
    "status_watchdog": "ERROR",
    "last_communication": "2026-03-11 14:25:10",
    "uptime": "2h 45m",
    "firmware_version": "v1.2.2",
    "config_version": "cfg-2026-03-09",
    "mac_address": "24:6F:28:DD:EE:FF",
    "ip_address": "192.168.1.102"
  },
]
# Node object fields:
Each node object contains:
  - name: string
  - role: string
  - node_type: string (e.g. "WebServer", "DMX", "MIDI")
  - node_sequence: integer
  - status_watchdog: string
  - last_communication: string
  - uptime: string
  - firmware_version: string
  - config_version: string
  - mac_address: string
  - ip_address: string
```

# Nodes Page

## Opening page

**Endpoint:** `GET /api/v1/nodes_info`

**Description:** Returns an array of all nodes with their names and current MAC addresses. Used to display and edit node MACs in the Initialization page.

**Returns:** `200 OK` with JSON array of nodes.

**Response Specification:**
Array of node objects, each containing:

- `name` (string, required) – Node identifier (e.g., "Master", "WebServer")
- `mac_address` (string, required) – MAC address in format XX:XX:XX:XX:XX:XX

Example request:

```
GET /api/v1/nodes_info
```

Example response (JSON):

```json
[
  { "name": "Master", "mac_address": "24:6F:28:AA:BB:CC" },
  { "name": "WebServer", "mac_address": "24:6F:28:BB:CC:DD" },
  { "name": "GPIO Node", "mac_address": "24:6F:28:CC:DD:EE" },
  { "name": "Display Node", "mac_address": "24:6F:28:DD:EE:FF" }
]
```

## Save Button

**Endpoint:** `POST /api/v1/nodes_info`

**Description:** Updates the MAC addresses for all nodes. Expects a JSON array of MAC addresses (in the same order as returned by GET). Returns ack/nack. Only valid MAC addresses are accepted.

**Returns:** `200 OK` with `{ "ack": "ok" }` on success; `400 Bad Request` with `{ "ack": "nok" }` on error.

**Request Specification:**
Array of MAC address strings (one per node, in the same order as GET /api/v1/nodes_info):

- Each element (string, required) – MAC address in format XX:XX:XX:XX:XX:XX

**Response Specification:**

- `ack` (string, required) – Acknowledgment status ("ok" on success, "nok" on failure)

Example request:

```json
POST /api/v1/nodes_info

["24:6F:28:AA:BB:CC", "24:6F:28:BB:CC:DD", "24:6F:28:CC:DD:EE", "24:6F:28:DD:EE:FF"]
```

Example response (JSON):

```json
{ "ack": "ok" }
```

## Reboot Button

**Endpoint:** `POST /api/v1/reboot`

**Description:** Triggers a system reboot action. No request body or response is required. Used by the Reboot button on the Initialization page.

**Returns:** `204 No Content` on success.

**Request Specification:**
No request body required.

**Response Specification:**
No response body. HTTP status 204 No Content indicates success.

Example request:

```
POST /api/v1/reboot
```

Example response:

```
204 No Content
```

## Factory Reset

**Endpoint:** `POST /api/v1/factory_reset`

**Description:** Performs a factory reset on the web server (clears configuration, resets to defaults, may reboot).

**Returns:** `200 OK` with `{ "ack": "ok" }` on success; `{ "ack": "nok" }` on failure.

**Website Page:** Nodes page, press Factory Reset button.

**Request Specification:**
No request body required.

**Response Specification:**

- `ack` (string, required) – Acknowledgment status ("ok" on success, "nok" on failure)

Example request:

```
POST /api/v1/factory_reset
```

Example response (JSON):

```json
{ "ack": "ok" }
```

## Firmware Update

### POST /api/v1/firmware_chunk/{node_idx}

**Endpoint:** `POST /api/v1/firmware_chunk/{node_idx}`

**Description:** Upload a chunk of firmware for a given node (4KB per chunk). The frontend splits the firmware .bin file into 4KB chunks and sends them sequentially. After all chunks are sent, the frontend calls the finalize endpoint to assemble and flash the firmware.

**Request Specification:**

- **URL Parameter:** `node_idx` (required, integer) – Node index
- **Body:** JSON object containing:
  - `chunk` (integer, required) – Chunk index (starting from 0)
  - `data` (string, required) – Base64-encoded binary chunk data

**Response Specification:**

- `ack` (string, required) – Acknowledgment status ("ok" on success, "nok" on failure)
- `error` (string, optional) – Error message if ack is "nok"

**Upload Process:**

1. User selects .bin file in the Nodes page UI. JavaScript splits the file into 4KB chunks.
2. For each chunk, POST to `/api/v1/firmware_chunk/{node_idx}` with chunk index and base64 data. No checksum is used; TCP ensures integrity.
3. The UI handles retries for failed chunks and shows a live progress bar.
4. After all chunks, POST to `/api/v1/firmware_finish/{node_idx}`.
5. On success, the UI shows "Firmware updated!" for that node.

**Note:** During firmware upload, the frontend buffers live status updates (SSE) to prevent UI re-renders from interrupting the progress bar and button state. The latest status is rendered after upload completes.

Example request:

```json
POST /api/v1/firmware_chunk/1

{
  "chunk": 0,
  "data": "base64-encoded-chunk-data..."
}
```

Example response (JSON):

```json
{ "ack": "ok" }
```

### POST /api/v1/firmware_finish/{node_idx}

**Endpoint:** `POST /api/v1/firmware_finish/{node_idx}`

**Description:** Finalize firmware upload for a node, assemble and flash firmware.

**Request Specification:**

- **URL Parameter:** `node_idx` (required, integer) – Node index
- **Body:** (empty, no request body required)

**Response Specification:**

- `ack` (string, required) – Acknowledgment status ("ok" on success, "nok" on failure)
- `error` (string, optional) – Error message if ack is "nok"

Example request:

```
POST /api/v1/firmware_finish/1
```

Example response (JSON):

```json
{ "ack": "ok" }
```

# Security Page

## Send ESP-NOW Security Key

**Endpoint:** `POST /api/v1/esp_now_key`

**Description:** Sends the ESP-NOW security key from the web UI to the backend. The key is a 16-byte (uint8_t) array, scrambled using a fixed permutation for security and reversibility. The backend must descramble the key using the same order.

**Returns:** `200 OK` with `{ "ack": "ok" }` on success; `400 Bad Request` with `{ "ack": "nok" }` on error.

**Request Specification:**

- **Body:** JSON array of 16 hex strings representing scrambled key bytes:
  - Each element (string, required) – Hex string (e.g., "a1", "b2") representing a key byte
  - Scramble order (index mapping): [7, 2, 12, 0, 9, 5, 1, 14, 8, 3, 10, 6, 13, 11, 4, 15]
  - To descramble: `original[scramble_order[i]] = scrambled[i]` for i = 0..15

**Response Specification:**

- `ack` (string, required) – Acknowledgment status ("ok" on success, "nok" on failure)

Example request (URL + JSON):

```json
POST /api/v1/esp_now_key

["a1", "b2", "c3", "d4", "e5", "f6", "07", "18", "29", "3a", "4b", "5c", "6d", "7e", "8f", "90"]
```

**Response (JSON):**

```json
{ "ack": "ok" }
```

## Send Wi-Fi Password

**Endpoint:** `POST /api/v1/wifi_password`

**Description:** Sends the Wi-Fi password from the web UI to the backend. The password is a string (WPA2: 8-63 characters). The backend must validate and securely store or forward the password.

**Returns:** `200 OK` with `{ "ack": "ok" }` on success; `400 Bad Request` with `{ "ack": "nok" }` on error.

**Request Specification:**

- **Body:** JSON object containing:
  - `password` (string, required) – Wi-Fi password (WPA2: 8-63 characters)

**Response Specification:**

- `ack` (string, required) – Acknowledgment status ("ok" on success, "nok" on failure)

Example request (URL + JSON):

```json
POST /api/v1/wifi_password

{ "password": "mysecretwifi" }
```

**Response (JSON):**

```json
{ "ack": "ok" }
```

# Logging Page

## Logging Stream (SSE)

**Endpoint:** `GET /api/v1/logging`

**Description:** Streams real-time log lines using Server-Sent Events (SSE).

**Returns:** `200 OK` with `Content-Type: text/event-stream` and a stream of log lines. Connection remains open while the page is active.

**Request Specification:**
No request parameters or body required.

**Response Specification:**
Server-sent events containing individual log lines:

- `event` (string) – Event type ("") (default event type for plain text)
- `data` (string) – Log line containing timestamp and message (e.g., "2026-03-12 15:42:10 [INFO] System initialized")

**Frontend behavior:**

- The Logging page opens a connection to `/api/v1/logging` using EventSource.
- Each received line is appended to a large textarea for live display.
- The connection is closed when the page is unloaded.

Example request:

```
GET /api/v1/logging
```

Example event:

```
data: 2026-03-12 15:42:10 [INFO] System initialized
data: 2026-03-12 15:42:11 [DEBUG] WebServer started
data: 2026-03-12 15:42:12 [INFO] All nodes connected
```

# General Notes
