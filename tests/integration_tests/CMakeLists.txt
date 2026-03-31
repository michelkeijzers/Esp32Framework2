file(GLOB_RECURSE INTEGRATION_TEST_SOURCES "*.cpp")

add_executable(integration_tests ${INTEGRATION_TEST_SOURCES})

if(MSVC)
    set_property(TARGET integration_tests PROPERTY INTERPROCEDURAL_OPTIMIZATION FALSE)
    target_compile_options(integration_tests PRIVATE /GL-)
    target_link_options(integration_tests PRIVATE /LTCG:OFF)
    target_link_options(integration_tests PRIVATE /OPT:NOREF /OPT:NOICF)
endif()

target_include_directories(integration_tests PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../mocks
)

target_link_libraries(integration_tests
    production_lib
    GTest::gtest
    GTest::gmock
)

include(GoogleTest)
gtest_discover_tests(integration_tests)
