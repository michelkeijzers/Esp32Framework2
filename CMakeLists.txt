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
