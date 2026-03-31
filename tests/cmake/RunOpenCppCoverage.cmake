if(NOT DEFINED OPENCPPCOVERAGE_EXE)
    message(FATAL_ERROR "OPENCPPCOVERAGE_EXE must be provided")
endif()

if(NOT DEFINED SOURCE_ROOT)
    message(FATAL_ERROR "SOURCE_ROOT must be provided")
endif()

if(NOT DEFINED TEST_BUILD_DIR)
    message(FATAL_ERROR "TEST_BUILD_DIR must be provided")
endif()

set(UNIT_TEST_EXE "${TEST_BUILD_DIR}/unit_tests/Debug/unit_tests.exe")
set(INTEGRATION_TEST_EXE "${TEST_BUILD_DIR}/integration_tests/Debug/integration_tests.exe")
set(COVERAGE_HTML_DIR "${TEST_BUILD_DIR}/coverage_html")
set(COVERAGE_XML "${TEST_BUILD_DIR}/coverage.xml")
set(UNIT_COVERAGE_BINARY "${TEST_BUILD_DIR}/unit_tests.cov")
set(COMBINED_COVERAGE_BINARY "${TEST_BUILD_DIR}/combined_coverage.cov")

foreach(required_path
    "${OPENCPPCOVERAGE_EXE}"
    "${UNIT_TEST_EXE}"
    "${INTEGRATION_TEST_EXE}"
)
    if(NOT EXISTS "${required_path}")
        message(FATAL_ERROR "Required path not found: ${required_path}")
    endif()
endforeach()

file(REMOVE_RECURSE "${COVERAGE_HTML_DIR}")
file(REMOVE "${COVERAGE_XML}" "${UNIT_COVERAGE_BINARY}" "${COMBINED_COVERAGE_BINARY}")

file(TO_NATIVE_PATH "${OPENCPPCOVERAGE_EXE}" OPENCPPCOVERAGE_NATIVE)
file(TO_NATIVE_PATH "${SOURCE_ROOT}" SOURCE_ROOT_NATIVE)
file(TO_NATIVE_PATH "${SOURCE_ROOT}/main" SOURCES_NATIVE)
file(TO_NATIVE_PATH "${TEST_BUILD_DIR}" TEST_BUILD_NATIVE)
file(TO_NATIVE_PATH "${UNIT_TEST_EXE}" UNIT_TEST_NATIVE)
file(TO_NATIVE_PATH "${INTEGRATION_TEST_EXE}" INTEGRATION_TEST_NATIVE)
file(TO_NATIVE_PATH "${COVERAGE_HTML_DIR}" COVERAGE_HTML_NATIVE)
file(TO_NATIVE_PATH "${COVERAGE_XML}" COVERAGE_XML_NATIVE)
file(TO_NATIVE_PATH "${UNIT_COVERAGE_BINARY}" UNIT_COVERAGE_BINARY_NATIVE)
file(TO_NATIVE_PATH "${COMBINED_COVERAGE_BINARY}" COMBINED_COVERAGE_BINARY_NATIVE)

execute_process(
    COMMAND "${OPENCPPCOVERAGE_NATIVE}"
        --modules=${SOURCE_ROOT_NATIVE}
        --sources=${SOURCES_NATIVE}
        --working_dir=${TEST_BUILD_NATIVE}
        --export_type=binary:${UNIT_COVERAGE_BINARY_NATIVE}
        --
        "${UNIT_TEST_NATIVE}"
    RESULT_VARIABLE unit_result
)

if(NOT unit_result EQUAL 0)
    message(FATAL_ERROR "OpenCppCoverage failed while running unit_tests.exe (exit code ${unit_result})")
endif()

execute_process(
    COMMAND "${OPENCPPCOVERAGE_NATIVE}"
        --modules=${SOURCE_ROOT_NATIVE}
        --sources=${SOURCES_NATIVE}
        --working_dir=${TEST_BUILD_NATIVE}
        --input_coverage=${UNIT_COVERAGE_BINARY_NATIVE}
        --export_type=binary:${COMBINED_COVERAGE_BINARY_NATIVE}
        --export_type=html:${COVERAGE_HTML_NATIVE}
        --export_type=cobertura:${COVERAGE_XML_NATIVE}
        --
        "${INTEGRATION_TEST_NATIVE}"
    RESULT_VARIABLE integration_result
)

if(NOT integration_result EQUAL 0)
    message(FATAL_ERROR "OpenCppCoverage failed while running integration_tests.exe (exit code ${integration_result})")
endif()

set(COVERAGE_INDEX_HTML "${COVERAGE_HTML_DIR}/index.html")
if(EXISTS "${COVERAGE_INDEX_HTML}")
    string(TIMESTAMP COVERAGE_GENERATED_AT "%Y-%m-%d %H:%M:%S %z")
    file(READ "${COVERAGE_INDEX_HTML}" COVERAGE_HTML_CONTENT)

    # Inject a visible generation timestamp at the top of the report body.
    string(REGEX REPLACE
        "(<body[^>]*>)"
        "\\1\n<div style=\"background:#0f172a;color:#e2e8f0;padding:10px 14px;font:600 14px/1.4 Segoe UI,Arial,sans-serif;border-bottom:3px solid #38bdf8;\">Coverage generated at: ${COVERAGE_GENERATED_AT}</div>"
        COVERAGE_HTML_CONTENT
        "${COVERAGE_HTML_CONTENT}"
    )

    file(WRITE "${COVERAGE_INDEX_HTML}" "${COVERAGE_HTML_CONTENT}")
else()
    message(WARNING "Coverage index.html not found for timestamp injection: ${COVERAGE_INDEX_HTML}")
endif()

message(STATUS "Coverage HTML report: ${COVERAGE_HTML_DIR}/index.html")
message(STATUS "Coverage Cobertura XML: ${COVERAGE_XML}")