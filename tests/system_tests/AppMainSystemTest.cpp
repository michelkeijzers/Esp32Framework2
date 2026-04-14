#include <gtest/gtest.h>

#include "low_level_stubs/LowLevelStubCounters.hpp"

extern "C" void app_main(void);

TEST(AppMainSystemTest, AppMainBuildsMasterCompositionAndStartsNode) {
    low_level_stubs::reset_counters();

    app_main();

    const auto &calls = low_level_stubs::counters();
    EXPECT_GE(calls.esp_now_init_calls, 1);
    EXPECT_GE(calls.httpd_start_calls, 1);
    EXPECT_GE(calls.littlefs_mount_calls, 1);
    EXPECT_GE(calls.free_rtos_create_task_calls, 1);
}
