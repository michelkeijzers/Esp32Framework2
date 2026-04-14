# DEVELOPER INFO

# GitHub Repository

https://github.com/michelkeijzers/Esp32Framework2

# Docker

## Overview

The workspace includes a .devcontainer directory with a Dockerfile and devcontainer.json configuration.
The Dockerfile is based on the official espressif/idf:v6.0 image, providing the ESP-IDF toolchain for ESP32 development. It also installs additional tools like Python, pip, and curl for convenience.

To use Docker, run Docker Desktop (Personal), see https://docs.docker.com/desktop/install/windows-install/ for installation instructions. Then, open the project in Visual Studio Code and use the "Remote - Containers" extension to open the folder in a container. This will build the Docker image and start a container with the development environment set up.

## Installed Components

- **ESP-IDF Toolchain** - Complete ESP32 development environment from espressif/idf:v6.0
- **Python** - Programming language runtime
- **pip** - Python package manager
- **curl** - Command-line tool for transferring data with URLs
- **Git** - Version control system
- **Build tools** - Compilers and build utilities for ESP32 projects

# VS Code

## Overview

Visual Studio Code is the recommended IDE for development. It provides a rich set of features and extensions for ESP32 development, including integration with the ESP-IDF toolchain, debugging support, and code editing features.

## WSL2

WSL2 (Windows Subsystem for Linux) is an optional component that allows you to run a Linux environment directly on Windows. It can be used as an alternative to Docker for development, providing a more native Linux experience. To use WSL2, you need to install a Linux distribution from the Microsoft Store and set up the ESP-IDF toolchain within that environment.

## VS Code Extension

The following extension must be installed to use docker:

- **Remote - Containers** - For developing inside a Docker container

## VS Code Extensions

The following extensions are installed by the Docker container:

- **Espressif ESP-IDF Extension** (`espressif.esp-idf-extension`)
- **C/C++** (`ms-vscode.cpptools`)
- **CMake Tools** (`ms-vscode.cmake-tools`)
- **Coverage Gutters** (`ryanluker.vscode-coverage-gutters`)
- **GitLens** (`eamodio.gitlens`)
- **PlantUML** (`jebbs.plantuml`)
- **Prettier - Code formatter** (`esbenp.prettier-vscode`)
- **Python** (`ms-python.python`)
- **Docker** (`ms-azuretools.vscode-docker`)
- **REST Client** (`humao.rest-client`)
- **Markdown All in One** (`yzhang.markdown-all-in-one`)
- **Live Server** (`ritwickdey.liveserver`)

# ESP32 Build (F7)

## Overview

The F7 key is assigned for building. It performs the following actions:

1. Sets up the ESP-IDF environment (sources export.sh).
2. Checks for a stale CMake path in the build cache and reconfigures if needed.
3. Runs idf.py build to build the ESP32 project.
4. Runs integration tests and unit tests using ctest.
5. Prints the test results with progress and output on failure.
6. Prints coverage output, including the location of the HTML and XML coverage reports if they exist.

# Testing

## Overview

The following tests are performed:

1. Integration tests
2. Unit tests

## Integration Tests

Integration tests are run using ctest after the build process. They are designed to test the functionality of the ESP32 project in an integrated environment, ensuring that all components work together as expected. The test results are printed with progress and output on failure for easy debugging.

## Unit Tests

Unit tests are also run using ctest after the build process. They focus on testing individual components or functions of the ESP32 project in isolation, ensuring that each part works correctly on its own. Like integration tests, the results are printed with progress and output on failure for easy debugging.

## Coverage Reports

After running the build (F7) task, coverage reports are generated:

- [HTML Coverage Report](../../tests/build/coverage_html/index.html) — x Open in your browser for a visual summary.
- [Cobertura XML Report](tests/build/coverage.xml) — For CI tools and detailed analysis.

## Website

- Open a terminal in main/project_dmx_controller/website.
- Install dependencies (if not already): npm install
- Start the development server: npm run dev
- This will serve the site (including index.html and the simulator) on a local address (e.g., http://localhost:5173).
- Open the provided URL in your browser.
