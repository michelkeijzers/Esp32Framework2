#include <gtest/gtest.h>
#include "PresetManager.hpp"

// ─────────────────────────────────────────────────────────────
// Fixture
// ─────────────────────────────────────────────────────────────
struct PresetManagerTest : public ::testing::Test
{
    PresetManager pm;
};

// ─────────────────────────────────────────────────────────────
// get_presets / save_preset
// ─────────────────────────────────────────────────────────────

TEST_F(PresetManagerTest, GetPresetsEmptyInitially)
{
    EXPECT_TRUE(pm.get_presets().empty());
}

TEST_F(PresetManagerTest, SavePresetAddsNewEntry)
{
    Preset p(1, "Scene A", false);
    EXPECT_EQ(ESP_OK, pm.save_preset(p));
    ASSERT_EQ(1u, pm.get_presets().size());
    EXPECT_EQ(1, pm.get_presets()[0].number);
}

TEST_F(PresetManagerTest, SavePresetUpdatesExistingEntry)
{
    Preset p(1, "Scene A", false);
    pm.save_preset(p);

    Preset updated(1, "Scene A Updated", true);
    EXPECT_EQ(ESP_OK, pm.save_preset(updated));

    EXPECT_EQ(1u, pm.get_presets().size());
    EXPECT_EQ("Scene A Updated", pm.get_presets()[0].name);
    EXPECT_TRUE(pm.get_presets()[0].active);
}

TEST_F(PresetManagerTest, SaveMultiplePresets)
{
    pm.save_preset(Preset(1, "A", false));
    pm.save_preset(Preset(2, "B", false));
    pm.save_preset(Preset(3, "C", false));
    EXPECT_EQ(3u, pm.get_presets().size());
}

// ─────────────────────────────────────────────────────────────
// get_preset
// ─────────────────────────────────────────────────────────────

TEST_F(PresetManagerTest, GetPresetFoundReturnsPointer)
{
    pm.save_preset(Preset(5, "Five", false));
    Preset *p = pm.get_preset(5);
    ASSERT_NE(nullptr, p);
    EXPECT_EQ(5, p->number);
}

TEST_F(PresetManagerTest, GetPresetNotFoundReturnsNull)
{
    EXPECT_EQ(nullptr, pm.get_preset(99));
}

// ─────────────────────────────────────────────────────────────
// select_preset
// ─────────────────────────────────────────────────────────────

TEST_F(PresetManagerTest, SelectPresetFound)
{
    pm.save_preset(Preset(3, "C", false));
    EXPECT_EQ(ESP_OK, pm.select_preset(3));
}

TEST_F(PresetManagerTest, SelectPresetNotFoundReturnsError)
{
    EXPECT_EQ(ESP_ERR_NOT_FOUND, pm.select_preset(99));
}

// ─────────────────────────────────────────────────────────────
// blackout
// ─────────────────────────────────────────────────────────────

TEST_F(PresetManagerTest, BlackoutReturnsOk)
{
    EXPECT_EQ(ESP_OK, pm.blackout());
}

// ─────────────────────────────────────────────────────────────
// delete_preset
// ─────────────────────────────────────────────────────────────

TEST_F(PresetManagerTest, DeletePresetFound)
{
    pm.save_preset(Preset(7, "G", false));
    EXPECT_EQ(ESP_OK, pm.delete_preset(7));
    EXPECT_TRUE(pm.get_presets().empty());
}

TEST_F(PresetManagerTest, DeletePresetNotFoundReturnsError)
{
    EXPECT_EQ(ESP_ERR_NOT_FOUND, pm.delete_preset(42));
}

// ─────────────────────────────────────────────────────────────
// move_preset
// ─────────────────────────────────────────────────────────────

TEST_F(PresetManagerTest, MovePresetUpSwapsWithPrevious)
{
    pm.save_preset(Preset(1, "A", false));
    pm.save_preset(Preset(2, "B", false));
    EXPECT_EQ(ESP_OK, pm.move_preset(2, true)); // move preset #2 up
    EXPECT_EQ(2, pm.get_presets()[0].number);
    EXPECT_EQ(1, pm.get_presets()[1].number);
}

TEST_F(PresetManagerTest, MovePresetDownSwapsWithNext)
{
    pm.save_preset(Preset(1, "A", false));
    pm.save_preset(Preset(2, "B", false));
    EXPECT_EQ(ESP_OK, pm.move_preset(1, false)); // move preset #1 down
    EXPECT_EQ(2, pm.get_presets()[0].number);
    EXPECT_EQ(1, pm.get_presets()[1].number);
}

TEST_F(PresetManagerTest, MovePresetUpAtBoundaryIsNoOp)
{
    pm.save_preset(Preset(1, "A", false));
    pm.save_preset(Preset(2, "B", false));
    EXPECT_EQ(ESP_OK, pm.move_preset(1, true)); // already at top
    EXPECT_EQ(1, pm.get_presets()[0].number);   // unchanged
}

TEST_F(PresetManagerTest, MovePresetNotFoundReturnsError)
{
    EXPECT_EQ(ESP_ERR_NOT_FOUND, pm.move_preset(99, true));
}

// ─────────────────────────────────────────────────────────────
// swap_activation
// ─────────────────────────────────────────────────────────────

TEST_F(PresetManagerTest, SwapActivationTogglesFlag)
{
    pm.save_preset(Preset(4, "D", false));
    EXPECT_EQ(ESP_OK, pm.swap_activation(4));
    EXPECT_TRUE(pm.get_preset(4)->active);
    EXPECT_EQ(ESP_OK, pm.swap_activation(4));
    EXPECT_FALSE(pm.get_preset(4)->active);
}

TEST_F(PresetManagerTest, SwapActivationNotFoundReturnsError)
{
    EXPECT_EQ(ESP_ERR_NOT_FOUND, pm.swap_activation(77));
}

// ─────────────────────────────────────────────────────────────
// get_active_presets
// ─────────────────────────────────────────────────────────────

TEST_F(PresetManagerTest, GetActivePresetsFiltersCorrectly)
{
    pm.save_preset(Preset(1, "A", true));
    pm.save_preset(Preset(2, "B", false));
    pm.save_preset(Preset(3, "C", true));

    auto active = pm.get_active_presets();
    ASSERT_EQ(2u, active.size());
    EXPECT_EQ(1, active[0]);
    EXPECT_EQ(3, active[1]);
}

TEST_F(PresetManagerTest, GetActivePresetsEmptyWhenNoneActive)
{
    pm.save_preset(Preset(1, "A", false));
    EXPECT_TRUE(pm.get_active_presets().empty());
}

// ─────────────────────────────────────────────────────────────
// commit
// ─────────────────────────────────────────────────────────────

TEST_F(PresetManagerTest, CommitReturnsOk)
{
    pm.save_preset(Preset(1, "A", false));
    EXPECT_EQ(ESP_OK, pm.commit());
}
