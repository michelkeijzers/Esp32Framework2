#!/usr/bin/env python3
"""Start the local API simulator if port 8080 is not already in use."""

from __future__ import annotations

import os
import socket
import subprocess
import sys
import time
import venv
from pathlib import Path

HOST = "127.0.0.1"
PORT = 8080
REQUIREMENTS_FILE = "api-simulator-requirements.txt"


def is_port_open(host: str, port: int, timeout: float = 0.3) -> bool:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.settimeout(timeout)
        return sock.connect_ex((host, port)) == 0


def has_flask_modules(python_exe: str) -> bool:
    check_code = (
        "import importlib.util as u; "
        "import sys; "
        "sys.exit(0 if u.find_spec('flask') and u.find_spec('flask_cors') else 1)"
    )
    result = subprocess.run(
        [python_exe, "-c", check_code],
        check=False,
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL,
    )
    return result.returncode == 0


def venv_python_path(venv_dir: Path) -> Path:
    if os.name == "nt":
        return venv_dir / "Scripts" / "python.exe"
    return venv_dir / "bin" / "python"


def ensure_runtime(website_dir: Path) -> str:
    current_python = sys.executable
    if has_flask_modules(current_python):
        return current_python

    venv_dir = website_dir / ".api-simulator-venv"
    requirements_path = website_dir / "scripts" / REQUIREMENTS_FILE
    if not requirements_path.exists():
        raise FileNotFoundError(f"Missing requirements file: {requirements_path}")

    if not venv_python_path(venv_dir).exists():
        print(f"[api_simulator] Creating virtual environment at {venv_dir}")
        venv.EnvBuilder(with_pip=True, clear=False, symlinks=True).create(venv_dir)

    runtime_python = str(venv_python_path(venv_dir))

    if not has_flask_modules(runtime_python):
        print("[api_simulator] Installing Flask dependencies for simulator")
        install_cmd = [
            runtime_python,
            "-m",
            "pip",
            "install",
            "--disable-pip-version-check",
            "--no-input",
            "-r",
            str(requirements_path),
        ]
        result = subprocess.run(install_cmd, check=False)
        if result.returncode != 0:
            raise RuntimeError("Failed to install Flask dependencies for simulator")

    return runtime_python


def start_server(website_dir: Path, python_executable: str) -> subprocess.Popen[bytes]:
    server_script = website_dir / "server.py"
    log_file = website_dir / ".api-simulator.log"

    if not server_script.exists():
        raise FileNotFoundError(f"Missing simulator script: {server_script}")

    stdout_handle = open(log_file, "ab")

    kwargs: dict[str, object] = {
        "cwd": str(website_dir),
        "stdin": subprocess.DEVNULL,
        "stdout": stdout_handle,
        "stderr": subprocess.STDOUT,
        "close_fds": True,
    }

    if os.name == "nt":
        creation_flags = 0
        creation_flags |= getattr(subprocess, "DETACHED_PROCESS", 0)
        creation_flags |= getattr(subprocess, "CREATE_NEW_PROCESS_GROUP", 0)
        kwargs["creationflags"] = creation_flags
    else:
        kwargs["start_new_session"] = True

    command = [python_executable, str(server_script)]
    return subprocess.Popen(command, **kwargs)


def main() -> int:
    website_dir = Path(__file__).resolve().parent.parent

    if is_port_open(HOST, PORT):
        print(f"[api_simulator] Already running on http://{HOST}:{PORT}")
        return 0

    try:
        runtime_python = ensure_runtime(website_dir)
        process = start_server(website_dir, runtime_python)
    except Exception as exc:  # pylint: disable=broad-except
        print(f"[api_simulator] Failed to launch: {exc}", file=sys.stderr)
        return 1

    # Give the process a short window to bind its port.
    for _ in range(10):
        time.sleep(0.2)
        if is_port_open(HOST, PORT):
            print(
                f"[api_simulator] Started on http://{HOST}:{PORT} "
                f"(pid={process.pid}, log=.api-simulator.log)"
            )
            return 0

    print(
        "[api_simulator] Process started but port did not open yet. "
        "Check website/.api-simulator.log",
        file=sys.stderr,
    )
    return 1


if __name__ == "__main__":
    raise SystemExit(main())
