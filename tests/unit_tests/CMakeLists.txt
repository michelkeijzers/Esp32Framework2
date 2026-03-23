file(GLOB_RECURSE UNIT_TEST_SOURCES "*.cpp")

add_executable(unit_tests ${UNIT_TEST_SOURCES})

if(MSVC)
    set_property(TARGET unit_tests PROPERTY INTERPROCEDURAL_OPTIMIZATION FALSE)
    target_compile_options(unit_tests PRIVATE /GL-)
    target_link_options(unit_tests PRIVATE /LTCG:OFF)
    target_link_options(unit_tests PRIVATE /OPT:NOREF /OPT:NOICF)
endif()

target_link_libraries(unit_tests
    production_lib
    GTest::gtest
    GTest::gmock
)

include(GoogleTest)
gtest_discover_tests(unit_tests)
