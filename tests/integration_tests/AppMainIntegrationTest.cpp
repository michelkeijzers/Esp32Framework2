#include <gtest/gtest.h>

#include "low_level_stubs/LowLevelStubCounters.hpp"

extern "C" void app_main(void);

TEST(AppMainIntegrationTest, AppMainInitializesAndStartsTopLevelFlow)
{
    low_level_stubs::reset_counters();

    app_main();

    const auto &calls = low_level_stubs::counters();
    std::cout << "[DEBUG] free_rtos_create_task_calls: " << calls.free_rtos_create_task_calls << std::endl;
    EXPECT_GE(calls.esp_now_init_calls, 1);
    EXPECT_GE(calls.httpd_start_calls, 1);
    EXPECT_GE(calls.littlefs_mount_calls, 1);
    EXPECT_EQ(calls.free_rtos_create_task_calls, 2) << "Expected exactly 2 RTOS tasks to be created (MasterTask and WebServerTask)";
}
