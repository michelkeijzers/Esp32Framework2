cmake_minimum_required(VERSION 3.20)
list(APPEND EXTRA_COMPONENT_DIRS "${CMAKE_SOURCE_DIR}/main/libraries/esp_littlefs")
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(Esp32Framework2)

# Build and run host tests from the root build without mixing ESP-IDF toolchains.
option(BUILD_HOST_TESTS_WITH_MAIN "Build/run host tests during main build" ON)
if(BUILD_HOST_TESTS_WITH_MAIN)
	add_custom_target(host_tests ALL
		# Self-heal stale/corrupted generated CMake metadata in host-test tree.
		COMMAND ${CMAKE_COMMAND} -E rm -rf
			${CMAKE_SOURCE_DIR}/tests/build/CMakeFiles/3.30.2
			${CMAKE_SOURCE_DIR}/tests/build/_deps/googletest-subbuild/CMakeFiles/3.30.2
		COMMAND ${CMAKE_COMMAND} -S ${CMAKE_SOURCE_DIR}/tests -B ${CMAKE_SOURCE_DIR}/tests/build
		COMMAND ${CMAKE_COMMAND} --build ${CMAKE_SOURCE_DIR}/tests/build --config Release
		COMMAND ${CMAKE_CTEST_COMMAND} --test-dir ${CMAKE_SOURCE_DIR}/tests/build -C Release --output-on-failure --progress --timeout 120
		COMMENT "Configuring, building, and running host tests"
		VERBATIM
	)
endif()

# Coverage report — gcovr on Linux/container, OpenCppCoverage on Windows.
option(BUILD_HOST_COVERAGE_WITH_MAIN "Build/run host coverage during main build" OFF)
find_program(GCOVR_EXECUTABLE NAMES gcovr)
find_program(OPENCPPCOVERAGE_EXECUTABLE
	NAMES OpenCppCoverage.exe OpenCppCoverage
	HINTS
		"C:/OpenCppCoverage"
		"C:/Program Files/OpenCppCoverage"
		"C:/Program Files (x86)/OpenCppCoverage"
)

if(GCOVR_EXECUTABLE)
	message(STATUS "gcovr found: ${GCOVR_EXECUTABLE} — host_coverage will use gcovr (Linux/GCC)")
elseif(OPENCPPCOVERAGE_EXECUTABLE)
	message(STATUS "OpenCppCoverage found: ${OPENCPPCOVERAGE_EXECUTABLE}")
else()
	message(WARNING "Neither gcovr nor OpenCppCoverage found. host_coverage target will be a no-op.")
endif()

if(GCOVR_EXECUTABLE AND BUILD_HOST_COVERAGE_WITH_MAIN)
	add_custom_target(host_coverage ALL
		# Remove stale generated metadata before re-configuring host-test coverage build.
		COMMAND ${CMAKE_COMMAND} -E rm -rf
			${CMAKE_SOURCE_DIR}/tests/build/CMakeFiles/3.30.2
			${CMAKE_SOURCE_DIR}/tests/build/_deps/googletest-subbuild/CMakeFiles/3.30.2
		COMMAND ${CMAKE_COMMAND} -S ${CMAKE_SOURCE_DIR}/tests -B ${CMAKE_SOURCE_DIR}/tests/build
			-DCMAKE_BUILD_TYPE=Debug -DENABLE_GCOV=ON
		COMMAND ${CMAKE_COMMAND} --build ${CMAKE_SOURCE_DIR}/tests/build --target unit_tests
		COMMAND ${CMAKE_COMMAND} --build ${CMAKE_SOURCE_DIR}/tests/build --target integration_tests
		COMMAND ${CMAKE_COMMAND}
			-DGCOVR_EXE=${GCOVR_EXECUTABLE}
			-DSOURCE_ROOT=${CMAKE_SOURCE_DIR}
			-DTEST_BUILD_DIR=${CMAKE_SOURCE_DIR}/tests/build
			-P ${CMAKE_SOURCE_DIR}/tests/cmake/RunGcovr.cmake
		COMMENT "Build, run, and generate host-side coverage with gcovr (output: tests/build/coverage_html/index.html)"
		VERBATIM
	)
	if(BUILD_HOST_TESTS_WITH_MAIN)
		add_dependencies(host_coverage host_tests)
	endif()
elseif(OPENCPPCOVERAGE_EXECUTABLE AND BUILD_HOST_COVERAGE_WITH_MAIN)
	add_custom_target(host_coverage ALL
		# Remove stale generated metadata before re-configuring host-test coverage build.
		COMMAND ${CMAKE_COMMAND} -E rm -rf
			${CMAKE_SOURCE_DIR}/tests/build/CMakeFiles/3.30.2
			${CMAKE_SOURCE_DIR}/tests/build/_deps/googletest-subbuild/CMakeFiles/3.30.2
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
	if(BUILD_HOST_TESTS_WITH_MAIN)
		add_dependencies(host_coverage host_tests)
	endif()
elseif(BUILD_HOST_COVERAGE_WITH_MAIN)
	message(WARNING "BUILD_HOST_COVERAGE_WITH_MAIN is ON but no coverage tool (gcovr or OpenCppCoverage) found.")
	add_custom_target(host_coverage ALL
		COMMAND ${CMAKE_COMMAND} -E echo "No coverage tool found (gcovr or OpenCppCoverage). Skipping coverage."
		COMMENT "No coverage tool available"
		VERBATIM
	)
else()
	add_custom_target(host_coverage
		COMMAND ${CMAKE_COMMAND} -E echo "No coverage tool found (gcovr or OpenCppCoverage). Skipping coverage."
		COMMENT "No coverage tool available"
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

	if(WIN32)
		# Post-build step: launch Flask server if not already running (Windows only)
		add_custom_command(TARGET website_build POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E echo "Checking if Flask server is running on port 8080 ..."
			COMMAND powershell -Command "if (-not (Test-NetConnection -ComputerName 127.0.0.1 -Port 8080 -WarningAction SilentlyContinue).TcpTestSucceeded) { Start-Process -NoNewWindow -FilePath python -ArgumentList 'server.py' -WorkingDirectory '${WEBSITE_DIR}' } else { Write-Host 'Flask server already running.' }"
			COMMENT "Auto-launch Flask API simulator after website build (if not already running)"
			VERBATIM
		)
	endif()

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

	if(WIN32)
		# Flask simulator and combined dev targets use PowerShell — Windows only
		add_custom_target(website_simulator
			COMMAND ${CMAKE_COMMAND} -E echo "Starting Flask simulator (server.py) on http://localhost:8080 ..."
			COMMAND powershell -Command "if (-not (Test-NetConnection -ComputerName 127.0.0.1 -Port 8080 -WarningAction SilentlyContinue).TcpTestSucceeded) { python server.py } else { Write-Host 'Flask server already running.' }"
			WORKING_DIRECTORY ${WEBSITE_DIR}
			COMMENT "Run Flask API simulator for website (Ctrl+C to stop)"
			USES_TERMINAL
			VERBATIM
		)
		add_custom_target(website_dev_all
			COMMAND powershell -NoProfile -ExecutionPolicy Bypass -File "${CMAKE_SOURCE_DIR}/main/project_dmx_controller/run-dev.ps1"
			WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/main/project_dmx_controller
			COMMENT "Start both Flask simulator and Vite dev server (parallel, with port check)"
			USES_TERMINAL
			VERBATIM
		)
	endif()
endif()
