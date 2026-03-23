#include <gtest/gtest.h>
#include "PresetManager.hpp"

TEST(PresetManagerIntegrationTest, FullCrudWorkflow)
{
    PresetManager pm;

    EXPECT_EQ(ESP_OK, pm.save_preset(Preset(1, "A", false)));
    EXPECT_EQ(ESP_OK, pm.save_preset(Preset(2, "B", true)));
    EXPECT_EQ(ESP_OK, pm.save_preset(Preset(3, "C", false)));
    EXPECT_EQ(3u, pm.get_presets().size());

    EXPECT_EQ(ESP_OK, pm.select_preset(2));

    auto active = pm.get_active_presets();
    ASSERT_EQ(1u, active.size());
    EXPECT_EQ(2, active[0]);

    EXPECT_EQ(ESP_OK, pm.swap_activation(3));
    active = pm.get_active_presets();
    ASSERT_EQ(2u, active.size());
    EXPECT_EQ(2, active[0]);
    EXPECT_EQ(3, active[1]);

    EXPECT_EQ(ESP_OK, pm.move_preset(3, true));
    EXPECT_EQ(3, pm.get_presets()[1].number);

    EXPECT_EQ(ESP_OK, pm.delete_preset(1));
    EXPECT_EQ(2u, pm.get_presets().size());

    EXPECT_EQ(ESP_OK, pm.blackout());
    EXPECT_EQ(ESP_OK, pm.commit());
}
