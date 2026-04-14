# DMX Controller API Contract

This document describes the API contract which are specific to this project.
The general API contracts can be found in [common_api_contract.md](../requirements_and_dds/common_api_contract.md).

# Table of Contents

- [Control Page](#control-page)
  - [Active Preset Numbers](#get-active-preset-numbers)
  - [Select Preset](#select-preset)
  - [Blackout](#blackout)
- [DMX Presets Page](#dmx-presets-page)
  - [Presets](#get-presets)
  - [Save Preset](#save-preset)
  - [Move Preset Up](#move-preset-up)
  - [Move Preset Down](#move-preset-down)
  - [Delete Preset](#delete-preset)
  - [Insert Preset At](#insert-preset-at)
  - [Swap Preset Activation](#swap-preset-activation)
- [Edit Preset](#edit-preset)
  - [Preset Values](#preset-values)
- [Edit Value Page](#edit-value-page)
  - [Preset Value](#preset-value)
- [Configuration Page](#configuration-page)
  - [Load Button](#load-button)
  - [Save Button](#save-button)
  - [Presets/Circular Navigation Checkbox](#presetscircular-navigation-checkbox)

# Control Page

## Active Preset Numbers

**Endpoint:** `GET /api/v1/active_preset_numbers`

**Description:** Returns an array of all active preset numbers to display only active presets on the Control page. The backend (Flask or ESP32) provides the current active preset numbers.

**Returns:** `200 OK` with JSON array of preset numbers.

**Website Page:** Control page, when page is opened.

**Request Specification:**
No request parameters or body required.

**Response Specification:**
Array of integers representing active preset numbers:

- Each element (integer, required) – Preset number (1-20) that is currently active

Example request:

```
GET /api/v1/active_preset_numbers
```

Example response (JSON):

```json
[1, 2, 5, 7, 10, 12, 15, 18, 20]
```

## Select Preset

**Endpoint:** `POST /api/v1/select_preset/<preset_number>`

**Description:** Selects a DMX preset by number (1-20). The backend will look up the preset name and return it.

**Returns:** `200 OK` with JSON response containing the selected preset name; `400 Bad Request` on invalid preset (preset_number not in 1-20 range).

**Website Page:** Control page, press Preset button.

**Request Specification:**

- **URL Parameter:** `preset_number` (required, integer 1-20)

**Response Specification:**

- `preset_name` (string, required) – Name of the selected preset (e.g., "Warm White")

Example request:

```
POST /api/v1/select_preset/1
```

Example response:

```json
{ "preset_name": "Warm White" }
```

## Blackout

**Endpoint:** `POST /api/v1/blackout`

**Description:** Activates blackout mode (all DMX light off). It is a special preset with a fixed name "Blackout".

**Returns:** `200 OK` with `{ "preset_name": "Blackout" }` on success.

**Website Page:** Control page, press Blackout button.

**Request Specification:**
No request body required.

**Response Specification:**

- `preset_name` (string, required) – Name of the blackout preset (always "Blackout")

Example request:

```
POST /api/v1/blackout
```

Example response (JSON):

```json
{ "preset_name": "Blackout" }
```

# DMX Presets Page

## Presets

**Endpoint:** `GET /api/v1/presets`

**Description:** Returns an array of all DMX presets with their names and active states to display the full Presets list. The backend (Flask or ESP32) provides the current names and activation status.

**Returns:** `200 OK` with JSON array of preset objects.

**Website Page:** Presets page, when page is opened.

**Response Specification:**
Array of preset objects, each containing:

- `name` (string, required) – Name of the preset (e.g., "Warm White")
- `active` (boolean, required) – Whether the preset is currently active

Example request:

```
GET /api/v1/presets
```

Example response (JSON):

```json
[
  { "name": "Warm White", "active": true },
  { "name": "Cool Blue", "active": false },
  { "name": "Party Mode", "active": false }
]
```

## Save Preset

**Endpoint:** `PUT /api/v1/save_preset/<preset_number>`

**Description:** Saves the current preset (name and DMX values) for the given preset number. Expects the updated preset data in the request body (JSON). Returns ack/nack.

**Returns:** `200 OK` with `{ "ack": "ok" }` on success; `400 Bad Request` with `{ "ack": "nok" }` on error.

**Request Specification:**

- **URL Parameter:** `preset_number` (required, integer 1-20)
- **Body:** JSON object containing:
  - `name` (string, required) – Name of the preset (e.g., "Cool Blue")
  - `dmx_values` (array of integers, required) – Array of 512 DMX channel values (0–255)

**Response Specification:**

- `ack` (string, required) – "ok" on success, "nok" on failure

Example request:

```
PUT /api/v1/save_preset/2

{
  "name": "Cool Blue",
  "dmx_values": [0, 128, 255, 0, 0, ..., 0]
}
```

Example response (JSON):

```json
{ "ack": "ok" }
```

## Move Preset Up

**Endpoint:** `PUT /api/v1/presets/<preset_index>/move_up`

**Description:** Moves the preset at the given index up by one position. Returns the updated preset list.
**Returns:** `200 OK` with updated preset list; `400 Bad Request` on invalid index.

**Request Specification:**

- **URL Parameter:** `preset_index` (required, integer 0-based index)

**Response Specification:**
Array of preset objects (same as Get Presets) with presets reordered after the move.

Example request:

```
PUT /api/v1/presets/2/move_up
```

Example response (JSON):

```json
[
  { "name": "Warm White", "active": true },
  { "name": "Party Mode", "active": false },
  { "name": "Cool Blue", "active": false }
]
```

## Move Preset Down

**Endpoint:** `PUT /api/v1/presets/<preset_index>/move_down`

**Description:** Moves the preset at the given index down by one position. Returns the updated preset list.

**Returns:** `200 OK` with updated preset list; `400 Bad Request` on invalid index.

**Request Specification:**

- **URL Parameter:** `preset_index` (required, integer 0-based index)

**Response Specification:**

Array of preset objects (same as Get Presets) with presets reordered after the move.

Example request:

```
PUT /api/v1/presets/1/move_down
```

Example response (JSON):

```json
[
  { "name": "Warm White", "active": true },
  { "name": "Party Mode", "active": false },
  { "name": "Cool Blue", "active": false }
]
```

## Delete Preset

**Endpoint:** `DELETE /api/v1/presets/<preset_index>`

**Description:** Deletes the preset at the given index. Returns the updated preset list.

**Returns:** `200 OK` with updated preset list; `400 Bad Request` on invalid index.

**Request Specification:**

- **URL Parameter:** `preset_index` (required, integer 0-based index)

**Response Specification:**
Array of preset objects (same as Get Presets) with the specified preset removed.

Example request:

```
DELETE /api/v1/presets/2
```

Example response (JSON):

```json
[
  { "name": "Warm White", "active": true },
  { "name": "Cool Blue", "active": false }
]
```

## Insert Preset At

**Endpoint:** `POST /api/v1/presets/<preset_index>/insert_at`

**Description:** Inserts a new preset at the given index. Returns the updated preset list.

**Returns:** `200 OK` with updated preset list; `400 Bad Request` on invalid index.

**Request Specification:**

- **URL Parameter:** `preset_index` (required, integer 0-based index)
- **Body:** JSON object containing:
  - `name` (string, optional) – Name of the new preset (default: empty string)
  - `dmx_values` (array of integers, optional) – DMX values for new preset (default: all zeros)

**Response Specification:**
Array of preset objects (same as Get Presets) with the new preset inserted at the specified index.

Example request:

```json
POST /api/v1/presets/1/insert_at

{
  "name": "New Preset",
  "dmx_values": [0, 0, 0, ..., 0]
}
```

Example response (JSON):

```json
[
  { "name": "Warm White", "active": true },
  { "name": "New Preset", "active": false },
  { "name": "Cool Blue", "active": false },
  { "name": "Party Mode", "active": false }
]
```

## Swap Preset Activation

**Endpoint:** `PUT /api/v1/presets/<preset_index>/swap_activation`

**Description:** Toggles the activation state of the preset at the given index. Returns the updated preset list.

**Returns:** `200 OK` with updated preset list; `400 Bad Request` on invalid index.

**Request Specification:**

- **URL Parameter:** `preset_index` (required, integer 0-based index)

**Response Specification:**
Array of preset objects (same as Get Presets) with the activation state of the specified preset toggled.

Example request:

```
PUT /api/v1/presets/2/swap_activation
```

Example response (JSON):

```json
[
  { "name": "Warm White", "active": true },
  { "name": "Cool Blue", "active": false },
  { "name": "Party Mode", "active": true }
]
```

# Edit Preset

## Preset Values

**Endpoint:** `GET /api/v1/preset_values/<preset_number>`

**Description:** Returns an array of 512 DMX values (0–255) for the specified preset number. Used to display or edit all DMX channel values for a given preset in the frontend.

**Returns:** `200 OK` with JSON array of DMX values if preset exists; `400 Bad Request` if preset not found.

**Request Specification:**

- **URL Parameter:** `preset_number` (required, integer 1-20)

**Response Specification:**
Array of 512 integers (0-255), where each element represents a DMX channel value:

- Index 0-511 represents DMX channels 1-512
- Each value is an integer 0-255 representing the intensity/brightness

**Example request:**

```
GET /api/v1/preset_values/2
```

**Example response:**

```json
[0, 128, 255, 0, 0, 64, 192, ..., 0]
```

# Edit Value Page

## Preset Value

**Endpoint:** `PUT /api/v1/preset_value/<preset>/<index>/<value>`

**Description:** Sets the DMX value for a specific preset and channel.

**Returns:** `200 OK` with updated value object on success; `400 Bad Request` on invalid input (out of range values or invalid preset).

**Request Specification:**

- **URL Parameters:**
  - `preset` (required, integer 1-20) – Preset number
  - `index` (required, integer 0-511) – DMX channel index (0-based, 512 total channels)
  - `value` (required, integer 0-255) – DMX intensity value

**Response Specification:**

- `index` (integer, required) – The DMX channel index that was updated
- `value` (integer, required) – The new DMX value (0-255)
- `preset` (integer, required) – The preset number

Example request:

```
PUT /api/v1/preset_value/2/45/128
```

Example response (JSON):

```json
{
  "preset": 2,
  "index": 45,
  "value": 128
}
```

# Configuration Page

## Load Button

**Endpoint:** `GET /api/v1/configuration`

**Description:** Returns all configuration settings as a JSON object. Used to load configuration in the UI.

**Returns:** `200 OK` with JSON object of config.

**Response Specification:**
JSON object containing all configuration settings:

- `circular navigation` (boolean, required) – Enable/disable circular navigation for preset lists

Example request:

```
GET /api/v1/configuration
```

Example response (JSON):

```json
{
  "circular navigation": true
}
```

## Save Button

**Endpoint:** `PUT /api/v1/configuration`

**Description:** Saves all configuration settings. Expects a JSON object with all config fields. Returns ack ("ok"/"nok").

**Returns:** `200 OK` with `{ "ack": "ok" }` on success; `400 Bad Request` with `{ "ack": "nok" }` on error.

**Request Specification:**
JSON object containing configuration fields to save:

- `circular navigation` (boolean, required) – Enable/disable circular navigation for preset lists

**Response Specification:**

- `ack` (string, required) – Acknowledgment status ("ok" on success, "nok" on failure)

Example request (URL + JSON):

```json
PUT /api/v1/configuration

{
  "circular navigation": false
}
```

Example response (JSON):

```json
{ "ack": "ok" }
```

## Presets/Circular Navigation Checkbox

**Endpoint:** `PUT /api/v1/configuration_presets_circular_navigation`

**Description:** Sets the circular navigation boolean for presets. Expects `{ "state": true|false }` in the request body. Returns ack/nack.

**Returns:** `200 OK` with `{ "ack": "ok" }` on success; `400 Bad Request` with `{ "ack": "nok" }` on error.

**Request Specification:**
JSON object containing the circular navigation state:

- `state` (boolean, required) – Enable (true) or disable (false) circular navigation

**Response Specification:**

- `ack` (string, required) – Acknowledgment status ("ok" on success, "nok" on failure)

Example request (URL + JSON):

```json
PUT /api/v1/configuration_presets_circular_navigation

{ "state": true }
```

Example response (JSON):

```json
{ "ack": "ok" }
```
