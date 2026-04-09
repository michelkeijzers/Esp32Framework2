#pragma once

namespace low_level_stubs {
struct Counters {
    int esp_now_init_calls = 0;
    int httpd_start_calls = 0;
    int free_rtos_create_task_calls = 0;
    int free_rtos_get_tick_count_calls = 0;
    int littlefs_mount_calls = 0;
};

Counters &counters();
void reset_counters();
} // namespace low_level_stubs
