# RunGcovr.cmake — Linux/GCC coverage report using gcovr
# Called by the host_coverage target with:
#   -DGCOVR_EXE=<path>  -DSOURCE_ROOT=<workspace>  -DTEST_BUILD_DIR=<tests/build>

foreach(var GCOVR_EXE SOURCE_ROOT TEST_BUILD_DIR)
    if(NOT DEFINED ${var})
        message(FATAL_ERROR "${var} must be provided")
    endif()
endforeach()

set(UNIT_TEST_EXE        "${TEST_BUILD_DIR}/unit_tests/unit_tests")
set(INTEGRATION_TEST_EXE "${TEST_BUILD_DIR}/integration_tests/integration_tests")
set(COVERAGE_HTML_DIR    "${TEST_BUILD_DIR}/coverage_html")
set(COVERAGE_XML         "${TEST_BUILD_DIR}/coverage.xml")

foreach(exe "${UNIT_TEST_EXE}" "${INTEGRATION_TEST_EXE}")
    if(NOT EXISTS "${exe}")
        message(FATAL_ERROR "Test executable not found: ${exe}")
    endif()
endforeach()

# Run tests through CTest so we get per-test progress, parallelism, and a hard timeout.
if(NOT DEFINED CMAKE_CTEST_COMMAND)
    find_program(CMAKE_CTEST_COMMAND NAMES ctest REQUIRED)
endif()

message(STATUS "Running host tests for coverage with ctest (timeout: 30s per test, parallel)...")
execute_process(
    COMMAND "${CMAKE_CTEST_COMMAND}"
        --test-dir "${TEST_BUILD_DIR}"
        -C Debug
        --output-on-failure
        --progress
        --timeout 30
        -j $ENV{CTEST_PARALLEL_LEVEL}
    WORKING_DIRECTORY "${TEST_BUILD_DIR}"
    RESULT_VARIABLE ctest_result
    TIMEOUT 300
)
message(STATUS "ctest completed with exit code: ${ctest_result}")
if(NOT ctest_result EQUAL 0)
    message(FATAL_ERROR "ctest failed while collecting coverage data (exit ${ctest_result})")
endif()

# Generate HTML + Cobertura XML reports
file(REMOVE_RECURSE "${COVERAGE_HTML_DIR}")
file(REMOVE "${COVERAGE_XML}")
file(MAKE_DIRECTORY "${COVERAGE_HTML_DIR}")

message(STATUS "Generating gcovr coverage report (timeout: 60s) ...")
)
message(STATUS "gcovr completed with exit code: ${gcovr_result}")
execute_process(
    COMMAND "${GCOVR_EXE}"
        --root "${SOURCE_ROOT}/main"
        --filter "${SOURCE_ROOT}/main/.*"
        --html-details "${COVERAGE_HTML_DIR}/index.html"
        --xml "${COVERAGE_XML}"
        --print-summary
        "${TEST_BUILD_DIR}"
    WORKING_DIRECTORY "${TEST_BUILD_DIR}"
    RESULT_VARIABLE gcovr_result
    TIMEOUT 60
)
if(NOT gcovr_result EQUAL 0)
    message(FATAL_ERROR "gcovr failed (exit ${gcovr_result})")
endif()

message(STATUS "Coverage HTML report: ${COVERAGE_HTML_DIR}/index.html")
message(STATUS "Coverage Cobertura XML: ${COVERAGE_XML}")
