cmake_minimum_required(VERSION 3.20)
list(APPEND EXTRA_COMPONENT_DIRS "${CMAKE_SOURCE_DIR}/main/libraries/esp_littlefs")
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(Esp32Framework2)

# Build and run host tests from the root build without mixing ESP-IDF toolchains.
option(BUILD_HOST_TESTS_WITH_MAIN "Build/run host tests during main build" ON)
if(BUILD_HOST_TESTS_WITH_MAIN)
	add_custom_target(host_tests ALL
		COMMAND ${CMAKE_COMMAND} -S ${CMAKE_SOURCE_DIR}/tests -B ${CMAKE_SOURCE_DIR}/tests/build
		COMMAND ${CMAKE_COMMAND} --build ${CMAKE_SOURCE_DIR}/tests/build --config Release
		COMMAND ${CMAKE_CTEST_COMMAND} --test-dir ${CMAKE_SOURCE_DIR}/tests/build -C Release --output-on-failure
		COMMENT "Configuring, building, and running host tests"
		VERBATIM
	)
endif()

# Coverage report for host-side tests (unit/integration) using OpenCppCoverage.
find_program(OPENCPPCOVERAGE_EXECUTABLE
	NAMES OpenCppCoverage.exe OpenCppCoverage
	HINTS
		"C:/OpenCppCoverage"
		"C:/Program Files/OpenCppCoverage"
		"C:/Program Files (x86)/OpenCppCoverage"
)

if(OPENCPPCOVERAGE_EXECUTABLE)
	add_custom_target(host_coverage
		COMMAND ${CMAKE_COMMAND} -S ${CMAKE_SOURCE_DIR}/tests -B ${CMAKE_SOURCE_DIR}/tests/build -DCMAKE_BUILD_TYPE=Debug
		COMMAND ${CMAKE_COMMAND} --build ${CMAKE_SOURCE_DIR}/tests/build --config Debug --target unit_tests
		COMMAND ${CMAKE_COMMAND} --build ${CMAKE_SOURCE_DIR}/tests/build --config Debug --target integration_tests
		COMMAND ${CMAKE_COMMAND}
			-DOPENCPPCOVERAGE_EXE=${OPENCPPCOVERAGE_EXECUTABLE}
			-DSOURCE_ROOT=${CMAKE_SOURCE_DIR}
			-DTEST_BUILD_DIR=${CMAKE_SOURCE_DIR}/tests/build
			-P ${CMAKE_SOURCE_DIR}/tests/cmake/RunOpenCppCoverage.cmake
		COMMENT "Build, run, and generate host-side coverage with OpenCppCoverage (output: tests/build/coverage_html/index.html)"
		VERBATIM
	)
else()
	add_custom_target(host_coverage
		COMMAND ${CMAKE_COMMAND} -E echo "OpenCppCoverage.exe was not found. Install it or add it to PATH to run host_coverage."
		COMMAND ${CMAKE_COMMAND} -E false
		COMMENT "OpenCppCoverage is required for host-side coverage"
		VERBATIM
	)
endif()

set(WEBSITE_DIR "${CMAKE_SOURCE_DIR}/main/project_dmx_controller/website")
find_program(NPM_EXECUTABLE NAMES npm npm.cmd)

if(NPM_EXECUTABLE)
	add_custom_target(website_build
		COMMAND ${NPM_EXECUTABLE} install
		COMMAND ${NPM_EXECUTABLE} run build
		WORKING_DIRECTORY ${WEBSITE_DIR}
		COMMENT "Installing website dependencies and building with Vite"
		VERBATIM
	)

	# Post-build step: launch Flask server if not already running
	add_custom_command(TARGET website_build POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E echo "Checking if Flask server is running on port 8080 ..."
		COMMAND powershell -Command "if (-not (Test-NetConnection -ComputerName 127.0.0.1 -Port 8080 -WarningAction SilentlyContinue).TcpTestSucceeded) { Start-Process -NoNewWindow -FilePath python -ArgumentList 'server.py' -WorkingDirectory '${WEBSITE_DIR}' } else { Write-Host 'Flask server already running.' }"
		COMMENT "Auto-launch Flask API simulator after website build (if not already running)"
		VERBATIM
	)

	add_custom_target(website_dev
		COMMAND ${NPM_EXECUTABLE} install
		COMMAND ${NPM_EXECUTABLE} run dev
		WORKING_DIRECTORY ${WEBSITE_DIR}
		COMMENT "Starting website dev server with Vite"
		USES_TERMINAL
		VERBATIM
	)

	option(BUILD_WEBSITE_WITH_MAIN "Build website during main build" OFF)
	if(BUILD_WEBSITE_WITH_MAIN)
		add_custom_target(website_all ALL
			COMMAND ${NPM_EXECUTABLE} install
			COMMAND ${NPM_EXECUTABLE} run build
			WORKING_DIRECTORY ${WEBSITE_DIR}
			COMMENT "Building website with Vite during main build"
			VERBATIM
		)
	endif()

	# Add a custom target to run the Flask simulator server for the website (manual)
	add_custom_target(website_simulator
		COMMAND ${CMAKE_COMMAND} -E echo "Starting Flask simulator (server.py) on http://localhost:8080 ..."
		COMMAND powershell -Command "if (-not (Test-NetConnection -ComputerName 127.0.0.1 -Port 8080 -WarningAction SilentlyContinue).TcpTestSucceeded) { python server.py } else { Write-Host 'Flask server already running.' }"
		WORKING_DIRECTORY ${WEBSITE_DIR}
		COMMENT "Run Flask API simulator for website (Ctrl+C to stop)"
		USES_TERMINAL
		VERBATIM
	)

	# Combined dev target: launches both Flask and Vite dev server in parallel (via PowerShell script)
	add_custom_target(website_dev_all
		COMMAND powershell -NoProfile -ExecutionPolicy Bypass -File "${CMAKE_SOURCE_DIR}/main/project_dmx_controller/run-dev.ps1"
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/main/project_dmx_controller
		COMMENT "Start both Flask simulator and Vite dev server (parallel, with port check)"
		USES_TERMINAL
		VERBATIM
	)
endif()
