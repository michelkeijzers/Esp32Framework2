# SERVICE FUNCTIONALITY: SECURITY

# OVERVIEW

The Security page on the website allows users to change the ESP-NOW security key and the Wi-Fi password.

# SCENARIO 1: CHANGE ESP-NOW SECURITY KEY

## Overview

The user can set a new ESP-NOW security key (16 bytes) via the web UI. The key is sent in a scrambled order for security.

## Requirements

- FR-SEC-100: The system shall allow the user to change the ESP-NOW security key through the web UI.

## Design Decisions

- DD-SEC-001: The key is scrambled before transmission using a fixed index mapping for additional security.

## Website

- 16 input fields for key bytes and a button labeled <Send ESP-NOW Key>.

## Sequence Diagram

_To be implemented when slave nodes are available._

## API Contract

### POST /api/v1/esp_now_key

Sends a new ESP-NOW security key to the backend in scrambled order.

**Request Body:**

JSON array of 16 hex strings (e.g., ["a1", "b2", ...]) representing scrambled key bytes.

Scramble order (index mapping): [7, 2, 12, 0, 9, 5, 1, 14, 8, 3, 10, 6, 13, 11, 4, 15]

To descramble: `original[scramble_order[i]] = scrambled[i]` for i = 0..15

**Example Request:**

```http
POST /api/v1/esp_now_key
Content-Type: application/json

["a1", "b2", "c3", "d4", "e5", "f6", "07", "18", "29", "3a", "4b", "5c", "6d", "7e", "8f", "90"]
```

**Example Response:**

```json
{ "ack": "ok" }
```

Where:

- `ack`: "ok" on success, "nok" on failure.

## ESP-NOW Messages

_To be defined when slave nodes are implemented._

## RTOS Messages

| Message                    | Source              | Destination        | Fields           | Description                                                                     |
| -------------------------- | ------------------- | ------------------ | ---------------- | ------------------------------------------------------------------------------- |
| `SECURITY_SET_ESP_NOW_KEY` | MasterWebserverTask | MasterSecurityTask | `key` (16 bytes) | Sends the new ESP-NOW security key (plain, not encrypted) to the security task. |

# SCENARIO 2: CHANGE WI-FI PASSWORD

## Overview

The user can set a new Wi-Fi password (WPA2: 8-63 characters) via the web UI.

## Requirements

- FR-SEC-200: The system shall allow the user to change the Wi-Fi password through the web UI.
- FR-SEC-201: The Wi-Fi password is a string (WPA2: 8-63 characters).

## Design Decisions

_No special design decisions._

## Website

- Input field for Wi-Fi password and a button labeled <Send Password>.

## Sequence Diagram

_To be implemented._

## API Contract

### POST /api/v1/wifi_password

Sends a new Wi-Fi password to the backend.

**Request Body:**

JSON object:

```json
{
  "password": "mysecretwifi"
}
```

**Example Request:**

```http
POST /api/v1/wifi_password
Content-Type: application/json

{
  "password": "mysecretwifi"
}
```

**Example Response:**

```json
{ "ack": "ok" }
```

Where:

- `ack`: "ok" on success, "nok" on failure.

## ESP-NOW Messages

_To be defined when slave nodes are implemented._

## RTOS Messages

| Message                      | Source              | Destination        | Fields              | Description                                                               |
| ---------------------------- | ------------------- | ------------------ | ------------------- | ------------------------------------------------------------------------- |
| `SECURITY_SET_WIFI_PASSWORD` | MasterWebserverTask | MasterSecurityTask | `password` (string) | Sends the new Wi-Fi password (plain, not encrypted) to the security task. |
