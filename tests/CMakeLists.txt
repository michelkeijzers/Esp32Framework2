cmake_minimum_required(VERSION 3.20)

project(Esp32Framework2Tests CXX)

# Enable coverage reporting (GCC/Clang only)
option(COVERAGE "Enable coverage reporting" ON)
if(COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    message(STATUS "Building with coverage flags")
    add_compile_options(--coverage -O0)
    add_link_options(--coverage)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    # Prevent VS/vcpkg auto-link from injecting wildcard vcpkg libs into
    # test executables. This can mix multiple gtest providers and break
    # static test registration/discovery.
    set(CMAKE_VS_GLOBALS "VcpkgEnabled=false")
endif()

if(MSVC)
    # Keep test executables self-contained on Windows test hosts.
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# ──────────────────────────────────────────────────────────────
# Google Test via FetchContent (self-contained)
# ──────────────────────────────────────────────────────────────
set(CMAKE_NO_SYSTEM_FROM_IMPORTED ON)
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# ──────────────────────────────────────────────────────────────
# Paths
# ──────────────────────────────────────────────────────────────
set(MAIN_DIR "${CMAKE_CURRENT_LIST_DIR}/../main")

# ──────────────────────────────────────────────────────────────
# Production library
#   Includes all platform-independent .cpp files.
#   Excluded:
#     - EspNow/EspNvs/EspHttpServer/EspLittleFs/EspLogger.cpp
#       (concrete ESP-IDF hardware drivers, replaced by GMock in tests)
#     - Webserver.cpp (project_dmx_controller) – broken ../http_task/ paths
#     - ApiServer.cpp – complex integration class not directly unit-tested
# ──────────────────────────────────────────────────────────────
set(PROD_SOURCES
    ${MAIN_DIR}/common/bridge/master_bridge/MasterBridge.cpp
    ${MAIN_DIR}/common/bridge/slave_bridge/SlaveBridge.cpp
    ${MAIN_DIR}/common/node/Node.cpp
    ${MAIN_DIR}/common/service_tasks/logging_task/LoggingTask.cpp
    ${MAIN_DIR}/common/service_tasks/ota_task/OtaTask.cpp
    ${MAIN_DIR}/common/service_tasks/ServiceTasks.cpp
    ${MAIN_DIR}/common/service_tasks/status_task/StatusTask.cpp
    ${MAIN_DIR}/common/sub_tasks/SubTask.cpp
    ${MAIN_DIR}/common/tasks/RtosTask.cpp
    ${MAIN_DIR}/common/webserver_task/apis/ApiFirmware.cpp
    ${MAIN_DIR}/common/webserver_task/apis/ApiLogging.cpp
    ${MAIN_DIR}/common/webserver_task/apis/ApiNodes.cpp
    ${MAIN_DIR}/common/webserver_task/apis/ApiResponse.cpp
    ${MAIN_DIR}/common/webserver_task/apis/ApiSecurity.cpp
    ${MAIN_DIR}/common/webserver_task/apis/ApiStatus.cpp
    ${MAIN_DIR}/common/webserver_task/apis/ApiSystem.cpp
    ${MAIN_DIR}/common/webserver_task/apis/ApiValidator.cpp
    ${MAIN_DIR}/project_dmx_controller/webserver_task/WebserverTask.cpp
    ${MAIN_DIR}/project_dmx_controller/master_task/MasterNode.cpp
    ${MAIN_DIR}/project_dmx_controller/master_task/MasterTask.cpp
    ${MAIN_DIR}/project_dmx_controller/presets_task/PresetManager.cpp
    ${MAIN_DIR}/project_dmx_controller/slave_node/SlaveNode.cpp
    ${MAIN_DIR}/project_dmx_controller/slave_node/SlaveTask.cpp
    ${MAIN_DIR}/project_dmx_controller/sub_tasks/button_task/ButtonTask.cpp
    ${MAIN_DIR}/project_dmx_controller/sub_tasks/dmx_max3485_sender_task/DmxMax3485SenderTask.cpp
    ${MAIN_DIR}/project_dmx_controller/sub_tasks/gpio_led_task/GpioLedTask.cpp
    ${MAIN_DIR}/project_dmx_controller/sub_tasks/max7219_task/Max7219Task.cpp
    ${MAIN_DIR}/project_dmx_controller/sub_tasks/potmeter_task/PotmeterTask.cpp
    ${MAIN_DIR}/project_dmx_controller/webserver_task/apis/ApiConfig.cpp
    ${MAIN_DIR}/project_dmx_controller/webserver_task/apis/ApiConfiguration.cpp
    ${MAIN_DIR}/project_dmx_controller/webserver_task/apis/ApiPresets.cpp
    ${MAIN_DIR}/project_dmx_controller/webserver_task/apis/ApiPresetValues.cpp
)

add_library(production_lib STATIC ${PROD_SOURCES})

target_include_directories(production_lib PUBLIC
    # Root of main/ — lets includes like "common/tasks/RtosTask.hpp" resolve
    ${MAIN_DIR}
    # Common sub-directories
    ${MAIN_DIR}/common
    ${MAIN_DIR}/common/bridge/master_bridge
    ${MAIN_DIR}/common/bridge/slave_bridge
    ${MAIN_DIR}/common/esp
    # ESP stub headers (resolves bare #include "IEspHttpServer.hpp" etc.)
    ${MAIN_DIR}/common/esp/esp_http_server
    ${MAIN_DIR}/common/esp/esp_nvs
    ${MAIN_DIR}/common/esp/esp_now
    ${MAIN_DIR}/common/esp/esp_logger
    ${MAIN_DIR}/common/esp/esp_error
    ${MAIN_DIR}/common/esp/free_rtos
    ${MAIN_DIR}/common/esp/esp_file_systems
    # Context and webserver
    ${MAIN_DIR}/common/context
    ${MAIN_DIR}/common/service_tasks
    ${MAIN_DIR}/common/service_tasks/logging_task
    ${MAIN_DIR}/common/service_tasks/ota_task
    ${MAIN_DIR}/common/service_tasks/status_task
    ${MAIN_DIR}/common/sub_tasks
    ${MAIN_DIR}/common/webserver_task
    # Resolves bare #include "ApiResponse.hpp", "ApiValidator.hpp"
    ${MAIN_DIR}/common/webserver_task/apis
    # Project-specific directories
    ${MAIN_DIR}/project_dmx_controller
    ${MAIN_DIR}/project_dmx_controller/master_task
    # Resolves bare #include "IEspHttpServer.hpp" in ApiPresets/ApiConfig/etc.
    ${MAIN_DIR}/project_dmx_controller/slave_node
    ${MAIN_DIR}/project_dmx_controller/sub_tasks/dmx_max3485_sender_task
    ${MAIN_DIR}/project_dmx_controller/webserver_task/apis
    ${MAIN_DIR}/project_dmx_controller/presets_task
    # Mock headers directory
    ${CMAKE_CURRENT_LIST_DIR}/mocks
)

# ──────────────────────────────────────────────────────────────
# Test sub-projects
# ──────────────────────────────────────────────────────────────
enable_testing()
add_subdirectory(unit_tests)
add_subdirectory(integration_tests)
