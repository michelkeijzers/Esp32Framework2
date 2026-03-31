add_executable(system_tests
    AppMainSystemTest.cpp
    low_level_stubs/LowLevelStubCounters.cpp
    low_level_stubs/EspNowStub.cpp
    low_level_stubs/EspHttpServerStub.cpp
    low_level_stubs/EspNvsStub.cpp
    low_level_stubs/EspLittleFsStub.cpp
    low_level_stubs/EspLoggerStub.cpp
    low_level_stubs/FreeRtosFactoryStub.cpp

    ${MAIN_DIR}/main.cpp
    ${MAIN_DIR}/common/context/ContextFactory.cpp
    ${MAIN_DIR}/common/context/CommonApiFactory.cpp
    ${MAIN_DIR}/common/context/EspFactory.cpp
    ${MAIN_DIR}/common/context/EspNowFactory.cpp
    ${MAIN_DIR}/project_dmx_controller/master_task/DmxContextFactory.cpp
    ${MAIN_DIR}/project_dmx_controller/master_task/DmxApiFactory.cpp
    ${MAIN_DIR}/project_dmx_controller/web_server_task/ApiServer.cpp
)

target_include_directories(system_tests PRIVATE
    ${MAIN_DIR}
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(system_tests
    production_lib
    GTest::gtest
    GTest::gmock
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(system_tests)
