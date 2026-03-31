
import datetime

import base64

import time
import threading
import json
from flask import Flask, request, send_from_directory, jsonify, Response
import os
from flask_cors import CORS

# --- Flask App ---
app = Flask(__name__, static_folder=".")
CORS(app)

# --- Firmware upload state (in-memory, per node) ---
firmware_uploads = {}


# --- SSE: Node Status Stream ---
def event_stream():
    import datetime

    try:
        while True:
            # Update master's last_communication every 2 seconds
            master_last_comm = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
            nodes = [
                {
                    "name": "Master",
                    "role": "Master",
                    "node_type": "WebServer",
                    "node_sequence": 1,
                    "status_watchdog": "OK",
                    "last_communication": master_last_comm,
                    "uptime": "3h 12m",
                    "firmware_version": "v1.2.3",
                    "config_version": "cfg-2026-03-10",
                    "mac_address": "24:6F:28:AA:BB:CC",
                    "ip_address": "192.168.1.101",
                },
                {
                    "name": "Node",
                    "role": "Node",
                    "node_type": "WebServer",
                    "node_sequence": 2,
                    "status_watchdog": "ERROR",
                    "last_communication": "2026-03-11 14:25:10",
                    "uptime": "2h 45m",
                    "firmware_version": "v1.2.2",
                    "config_version": "cfg-2026-03-09",
                    "mac_address": "24:6F:28:DD:EE:FF",
                    "ip_address": "192.168.1.102",
                },
                {
                    "name": "Backup",
                    "role": "Node",
                    "node_type": "MIDI",
                    "node_sequence": 3,
                    "status_watchdog": "OK",
                    "last_communication": "2026-03-11 14:26:30",
                    "uptime": "1h 10m",
                    "firmware_version": "v1.2.1",
                    "config_version": "cfg-2026-03-08",
                    "mac_address": "24:6F:28:11:22:33",
                    "ip_address": "192.168.1.103",
                },
            ]
            sse_message = f"event: status\ndata: {json.dumps(nodes)}\n\n"
            print(f"[SSE] Sending: {sse_message.strip()}")
            yield sse_message
            time.sleep(2)
    except GeneratorExit:
        print("[SSE] Client disconnected, stopping event stream.")


@app.route("/api/v1/status/stream")
def api_status_stream():
    return Response(event_stream(), mimetype="text/event-stream")


# --- Node Status Endpoint ---
@app.route("/api/v1/status", methods=["GET"])
def api_status():
    # Example: 3 nodes with different values
    nodes = [
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
            "ip_address": "192.168.1.101",
        },
        {
            "name": "Node",
            "role": "Node",
            "node_type": "WebServer",
            "node_sequence": 2,
            "status_watchdog": "ERROR",
            "last_communication": "2026-03-11 14:25:10",
            "uptime": "2h 45m",
            "firmware_version": "v1.2.2",
            "config_version": "cfg-2026-03-09",
            "mac_address": "24:6F:28:DD:EE:FF",
            "ip_address": "192.168.1.102",
        },
        {
            "name": "Backup",
            "role": "Node",
            "node_type": "MIDI",
            "node_sequence": 3,
            "status_watchdog": "OK",
            "last_communication": "2026-03-11 14:26:30",
            "uptime": "1h 10m",
            "firmware_version": "v1.2.1",
            "config_version": "cfg-2026-03-08",
            "mac_address": "24:6F:28:11:22:33",
            "ip_address": "192.168.1.103",
        },
    ]
    return jsonify(nodes)


# Simple in-memory config for demonstration
CONFIG = {"circular navigation": False}
CONFIG_LOCK = threading.Lock()


# --- New Configuration API Endpoints ---
@app.route("/api/v1/configuration_presets_circular_navigation", methods=["PUT"])
def api_config_presets_circular_navigation():
    data = request.get_json(force=True)
    print(
        f"POST /api/v1/configuration_presets_circular_navigation: received state={data.get('state')}"
    )
    if not isinstance(data, dict) or "state" not in data:
        return jsonify({"ack": "nok"}), 400
    with CONFIG_LOCK:
        CONFIG["circular navigation"] = bool(data["state"])
    return jsonify({"ack": "ok"})


@app.route("/api/v1/configuration", methods=["GET", "PUT"])
def api_configuration():
    if request.method == "GET":
        with CONFIG_LOCK:
            return jsonify(
                {"circular navigation": CONFIG.get(
                    "circular navigation", False)}
            )
    elif request.method == "PUT":
        data = request.get_json(force=True)
        if not isinstance(data, dict) or "circular navigation" not in data:
            return jsonify({"ack": "nok"}), 400
        with CONFIG_LOCK:
            CONFIG["circular navigation"] = bool(data["circular navigation"])
        return jsonify({"ack": "ok"})


@app.route("/api/v1/wifi_password", methods=["POST"])
def api_wifi_password():
    data = request.get_json(force=True)
    # Expecting { "password": "..." }
    if not isinstance(data, dict) or "password" not in data:
        return jsonify({"ack": "nok"}), 400
    password = data["password"]
    # WPA2 password must be 8-63 chars
    if not isinstance(password, str) or not (8 <= len(password) <= 63):
        return jsonify({"ack": "nok"}), 400
    print(
        f"[SECURITY] Received Wi-Fi password: (hidden, length={len(password)})")
    # Here you would securely store or forward the password
    return jsonify({"ack": "ok"})


# ...existing code...
# Save preset endpoint (must be after app is defined)
@app.route("/api/v1/save_preset/<int:preset_number>", methods=["PUT"])
def save_preset(preset_number):
    data = request.get_json(force=True)
    # Basic validation
    if not isinstance(data, dict) or "name" not in data or "dmx_values" not in data:
        return jsonify({"ack": "nok"}), 400
    # Here you would save the preset name and DMX values to your storage
    print(
        f"Saving preset {preset_number}: name={data['name']}, dmx_values[0:4]={data['dmx_values'][:4]} ..."
    )
    # Simulate success
    return jsonify({"ack": "ok"})


@app.route("/api/v1/preset_values/<int:x>", methods=["GET"])
def preset_values(x):
    print(f"GET /api/v1/preset_values/{x}")
    # TODO: Replace with real preset data lookup
    # For now, return a dummy array with some nonzero values for demonstration
    values = [0] * 512
    if 1 <= x <= 20:
        # Example: set some values for demonstration
        values[0] = 134
        values[1] = 34
        values[2] = 68
        values[3] = 128
        values[10] = 255
        values[100] = 77
    return jsonify(values)


# Preset names mapping
PRESET_NAMES = {
    "1": "Warm White",
    "2": "Cool Blue",
    "3": "Party Mode",
    "4": "Sunset",
    "5": "Stage Wash",
    "6": "Strobe",
    "7": "Fade",
    "8": "Rainbow",
    "9": "UV",
    "10": "Spotlight",
    "11": "Chase",
    "12": "Pulse",
    "13": "Fire",
    "14": "Ice",
    "15": "Relax",
    "16": "Focus",
    "17": "Energy",
    "18": "Ambient",
    "19": "Accent",
    "20": "Custom Scene",
}

# Example: all presets inactive except the first
PRESET_ACTIVATIONS = [True] + [False] * (len(PRESET_NAMES) - 1)


@app.route("/api/v1/presets", methods=["GET"])
def api_presets():
    print("GET /api/v1/presets")
    names = list(PRESET_NAMES.values())
    activations = PRESET_ACTIVATIONS
    presets = [
        {"name": name, "active": active} for name, active in zip(names, activations)
    ]
    return jsonify(presets)


# --- DMX Preset Actions (stubs, update logic as needed) ---
@app.route("/api/v1/move_preset_up/<int:x>", methods=["PUT"])
def move_preset_up(x):
    print(f"Move preset up: {x}")
    # TODO: Implement logic to move preset up
    return api_presets()


@app.route("/api/v1/move_preset_down/<int:x>", methods=["PUT"])
def move_preset_down(x):
    print(f"Move preset down: {x}")
    # TODO: Implement logic to move preset down
    return api_presets()


@app.route("/api/v1/delete_preset/<int:x>", methods=["DELETE"])
def delete_preset(x):
    print(f"Delete preset: {x}")
    return api_presets()


@app.route("/api/v1/insert_preset_at/<int:x>", methods=["PUT"])
def insert_preset_at(x):
    print(f"Insert preset at: {x}")
    return api_presets()


@app.route("/api/v1/swap_preset_activation/<int:x>/<int:state>", methods=["PUT"])
def preset_swap_active(x, state):
    print(f"Swap preset activation {x} to {state}")
    # TODO: Update PRESET_ACTIVATIONS[x-1] = bool(state)
    return api_presets()


# Endpoint to set a DMX value for a preset
@app.route("/api/v1/preset_value/<int:preset>/<int:index>/<int:value>", methods=["PUT"])
def set_preset_value(preset, index, value):
    # TODO: Implement logic to update the DMX value for the given preset and index
    print(f"Set DMX value: preset={preset}, index={index}, value={value}")
    return jsonify({"index": index, "value": value})


# Example: all presets inactive except the first
PRESET_ACTIVATIONS = [True] + [False] * (len(PRESET_NAMES) - 1)


@app.route("/api/v1/select_preset/<int:x>", methods=["POST"])
def select_preset(x):
    print(f"/api/v1/select_preset/{x} called")
    return "", 200


# --- Blackout Endpoint ---
@app.route("/api/v1/blackout", methods=["POST"])
def api_blackout():
    print("/api/v1/blackout called")
    # Here you would trigger the blackout action (e.g., set all DMX values to 0)
    return "", 200


# --- Firmware upload endpoints ---
@app.route('/api/v1/firmware_chunk/<int:node_idx>', methods=['POST'])
def firmware_chunk(node_idx):
    data = request.get_json()
    chunk_num = data.get('chunk')
    b64 = data.get('data')
    print(
        f"[FW] Received chunk: node={node_idx}, chunk={chunk_num}, b64len={len(b64) if b64 else 0}")
    if b64 is None or chunk_num is None:
        print(
            f"[FW] Error: missing data (b64={b64 is not None}, chunk_num={chunk_num})")
        return {'ack': 'nok', 'error': 'missing data'}, 400
    try:
        chunk_bytes = base64.b64decode(b64)
    except Exception as e:
        print(f"[FW] Error: base64 decode failed: {e}")
        return {'ack': 'nok', 'error': 'base64 decode failed'}, 400
    if node_idx not in firmware_uploads:
        firmware_uploads[node_idx] = {}
    firmware_uploads[node_idx][chunk_num] = chunk_bytes
    print(
        f"[FW] Stored chunk {chunk_num} for node {node_idx} (size={len(chunk_bytes)})")
    return {'ack': 'ok'}


@app.route('/api/v1/firmware_finish/<int:node_idx>', methods=['POST'])
def firmware_finish(node_idx):
    print(f"[FW] Finalizing firmware upload for node {node_idx}")
    if node_idx not in firmware_uploads:
        print(f"[FW] Error: no upload for node {node_idx}")
        return {'ack': 'nok', 'error': 'no upload'}, 400
    chunks = firmware_uploads[node_idx]
    if not chunks:
        print(f"[FW] Error: no chunks for node {node_idx}")
        return {'ack': 'nok', 'error': 'no chunks'}, 400
    try:
        ordered = [chunks[i] for i in sorted(chunks)]
        fw_data = b''.join(ordered)
        fw_path = f'firmware_upload_node{node_idx}.bin'
        with open(fw_path, 'wb') as f:
            f.write(fw_data)
        print(f"[FW] Firmware written to {fw_path} (size={len(fw_data)})")
        del firmware_uploads[node_idx]
        return {'ack': 'ok'}
    except Exception as e:
        print(f"[FW] Error during finalize: {e}")
        return {'ack': 'nok', 'error': str(e)}, 500


@app.route("/", methods=["GET"])
def index():
    print("GET /")
    return send_from_directory(".", "index.html")


# Serve /common/web_server_task/website/* from the common directory
@app.route("/common/web_server_task/website/<path:path>", methods=["GET"])
def serve_common_website(path):
    common_dir = os.path.abspath(os.path.join(os.path.dirname(
        __file__), '../../common/web_server_task/website'))
    print(f"GET /common/web_server_task/website/{path} (from {common_dir})")
    return send_from_directory(common_dir, path)

# Serve all other static files from the local website directory


@app.route("/<path:path>", methods=["GET"])
def static_files(path):
    print(f"GET /{path}")
    if path in ("nodes.html", "security.html", "logging.html"):
        common_dir = os.path.abspath(
            os.path.join(os.path.dirname(__file__), "../../common/web_server_task/website")
        )
        common_file = os.path.join(common_dir, path)
        if os.path.exists(common_file):
            return send_from_directory(common_dir, path)
    return send_from_directory(".", path)


@app.route("/api/v1/nodes_info", methods=["GET", "POST"])
def api_nodes_info():
    if request.method == "GET":
        # Mocked node info: name and mac_address for each node
        nodes = [
            {"name": "Master", "mac_address": "24:6F:28:AA:BB:CC"},
            {"name": "WebServer", "mac_address": "24:6F:28:BB:CC:DD"},
            {"name": "GPIO Node", "mac_address": "24:6F:28:CC:DD:EE"},
            {"name": "Display Node", "mac_address": "24:6F:28:DD:EE:FF"},
        ]
        return jsonify(nodes)
    elif request.method == "POST":
        data = request.get_json(force=True)
        print("POST /api/v1/nodes_info: received data=", data)
        if not isinstance(data, list):
            return (
                jsonify(
                    {"ack": "nok", "error": "Expected a list of MAC addresses"}),
                400,
            )
        mac_regex = r"^([0-9A-Fa-f]{2}[:-]){5}([0-9A-Fa-f]{2})$"
        import re

        if not all(isinstance(mac, str) and re.match(mac_regex, mac) for mac in data):
            return jsonify({"ack": "nok", "error": "Invalid MAC address format"}), 400
        # Here you would save the MACs to persistent storage or config
        return jsonify({"ack": "ok"})


# --- Reboot Endpoint ---
@app.route("/api/v1/reboot", methods=["POST"])
def api_reboot():
    print("POST /api/v1/reboot called")
    # Here you would trigger a system reboot or send a command to hardware
    return ("", 204)  # No Content


# --- ESP-NOW Key Endpoint ---
@app.route("/api/v1/esp_now_key", methods=["POST"])
def api_esp_now_key():
    data = request.get_json(force=True)
    print("POST /api/v1/esp_now_key: received scrambled key=", data)
    # TODO: Descramble and distribute the key to ESP-NOW network
    if (
        not isinstance(data, list)
        or len(data) != 16
        or not all(isinstance(x, str) and len(x) == 2 for x in data)
    ):
        return (
            jsonify({"ack": "nok", "error": "Expected a list of 16 hex strings"}),
            400,
        )
    return jsonify({"ack": "ok"})


@app.route("/api/v1/logging")
def api_logging():
    def log_stream():
        try:
            while True:
                now = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
                print(f"[SSE] Sent log line: {now}")
                yield f"data: {now}\n\n"
                time.sleep(1)
        except GeneratorExit:
            print("[SSE] Logging client disconnected.")
    return Response(log_stream(), mimetype="text/event-stream")


@app.route("/api/v1/factory_reset", methods=["POST"])
def api_factory_reset():
    # Here you would add logic to perform a factory reset (e.g., clear config, reset NVS, etc.)
    # For demo, just return ok
    try:
        # TODO: Implement actual factory reset logic
        print("[API] Factory reset requested!")
        return jsonify({"ack": "ok"})
    except Exception as e:
        print(f"[API] Factory reset error: {e}")
        return jsonify({"ack": "nok"}), 500


# --- Active Preset Numbers Endpoint ---
@app.route("/api/v1/active_preset_numbers", methods=["GET"])
def api_active_preset_numbers():
    # Example: return all active preset numbers (simulate with 1, 2, 5, 7, 10, 12, 15, 18, 20)
    active_presets = [1, 2, 5, 7, 10, 12, 13, 15, 18, 20]
    return jsonify(active_presets)


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=8080)
