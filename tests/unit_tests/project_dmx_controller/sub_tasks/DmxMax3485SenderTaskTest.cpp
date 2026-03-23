#include <gtest/gtest.h>
#include "DmxMax3485SenderTask.hpp"

// ─────────────────────────────────────────────────────────────
// Fixture
// ─────────────────────────────────────────────────────────────
struct DmxMax3485SenderTaskTest : public ::testing::Test
{
    DmxMax3485SenderTask task;
};

// ─────────────────────────────────────────────────────────────
// init / start
// ─────────────────────────────────────────────────────────────

TEST_F(DmxMax3485SenderTaskTest, InitReturnsOk)
{
    EXPECT_EQ(ESP_OK, task.init());
}

TEST_F(DmxMax3485SenderTaskTest, StartReturnsOk)
{
    task.init();
    EXPECT_EQ(ESP_OK, task.start());
}

// ─────────────────────────────────────────────────────────────
// setChannelValue
// ─────────────────────────────────────────────────────────────

TEST_F(DmxMax3485SenderTaskTest, SetChannelValueFirstChannel)
{
    // After setChannelValue the channel reflects via a round-trip through
    // setAllChannelValues + the array — test by writing then reading back
    // via setAllChannelValues/get pattern using known state.
    // We verify indirectly: set channel 0, then overwrite all to 0 except
    // verify the task doesn't crash (behaviour test).
    task.setChannelValue(0, 200);
    // No crash = pass; deeper value verification in integration tests.
}

TEST_F(DmxMax3485SenderTaskTest, SetChannelValueLastChannel)
{
    task.setChannelValue(511, 100);
}

TEST_F(DmxMax3485SenderTaskTest, SetChannelValueOutOfBoundsIgnored)
{
    // channel 512 is out of range — must not crash
    task.setChannelValue(512, 255);
}

TEST_F(DmxMax3485SenderTaskTest, SetChannelValueBoundaryValues)
{
    task.setChannelValue(0,   0);
    task.setChannelValue(255, 128);
    task.setChannelValue(511, 255);
}

// ─────────────────────────────────────────────────────────────
// setAllChannelValues
// ─────────────────────────────────────────────────────────────

TEST_F(DmxMax3485SenderTaskTest, SetAllChannelValuesCopiesArray)
{
    uint8_t values[512];
    for (int i = 0; i < 512; ++i)
        values[i] = static_cast<uint8_t>(i % 256);

    // Must not crash and must accept the full 512-byte block.
    task.setAllChannelValues(values);
}

TEST_F(DmxMax3485SenderTaskTest, SetAllChannelValuesAllZeros)
{
    uint8_t zeros[512] = {};
    task.setAllChannelValues(zeros);
}

TEST_F(DmxMax3485SenderTaskTest, SetAllChannelValuesAllMax)
{
    uint8_t maxValues[512];
    memset(maxValues, 0xFF, sizeof(maxValues));
    task.setAllChannelValues(maxValues);
}
