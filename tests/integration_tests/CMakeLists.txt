file(GLOB_RECURSE INTEGRATION_TEST_SOURCES "*.cpp")

add_executable(integration_tests ${INTEGRATION_TEST_SOURCES})

target_sources(integration_tests PRIVATE
    ${MAIN_DIR}/main.cpp
    ${MAIN_DIR}/common/context/ContextFactory.cpp
    ${MAIN_DIR}/common/context/CommonApiFactory.cpp
    ${MAIN_DIR}/common/context/EspFactory.cpp
    ${MAIN_DIR}/common/context/EspNowFactory.cpp
    ${MAIN_DIR}/project_dmx_controller/master_task/DmxContextFactory.cpp
    ${MAIN_DIR}/project_dmx_controller/master_task/DmxApiFactory.cpp
    ${MAIN_DIR}/project_dmx_controller/web_server_task/ApiServer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../system_tests/low_level_stubs/LowLevelStubCounters.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../system_tests/low_level_stubs/EspNowStub.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../system_tests/low_level_stubs/EspHttpServerStub.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../system_tests/low_level_stubs/EspNvsStub.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../system_tests/low_level_stubs/EspLittleFsStub.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../system_tests/low_level_stubs/EspLoggerStub.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../system_tests/low_level_stubs/FreeRtosFactoryStub.cpp
)

if(MSVC)
    set_property(TARGET integration_tests PROPERTY INTERPROCEDURAL_OPTIMIZATION FALSE)
    target_compile_options(integration_tests PRIVATE /GL-)
    target_link_options(integration_tests PRIVATE /LTCG:OFF)
    target_link_options(integration_tests PRIVATE /OPT:NOREF /OPT:NOICF)
endif()

target_include_directories(integration_tests PRIVATE
    ${MAIN_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../mocks
    ${CMAKE_CURRENT_LIST_DIR}/../system_tests
)

target_link_libraries(integration_tests
    production_lib
    GTest::gtest
    GTest::gmock
)

include(GoogleTest)
gtest_discover_tests(integration_tests)
