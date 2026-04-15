#include <gtest/gtest.h>

#include "project_dmx_controller/function_tasks/button_task/IButtonTask.hpp"
#include "project_dmx_controller/function_tasks/gpio_led_task/IGpioLedTask.hpp"
#include "project_dmx_controller/function_tasks/max7219_task/IMax7219Task.hpp"
#include "project_dmx_controller/function_tasks/potmeter_task/IPotmeterTask.hpp"

namespace {

class TestButtonTask final : public IButtonTask {
   public:
    esp_err_t init() override {
        initCalled = true;
        return ESP_OK;
    }

    esp_err_t start() override {
        startCalled = true;
        return ESP_OK;
    }

    bool initCalled{false};
    bool startCalled{false};
};

class TestGpioLedTask final : public IGpioLedTask {
   public:
    esp_err_t init() override {
        initCalled = true;
        return ESP_OK;
    }

    esp_err_t start() override {
        startCalled = true;
        return ESP_OK;
    }

    bool initCalled{false};
    bool startCalled{false};
};

class TestMax7219Task final : public IMax7219Task {
   public:
    esp_err_t init() override {
        initCalled = true;
        return ESP_OK;
    }

    esp_err_t start() override {
        startCalled = true;
        return ESP_OK;
    }

    bool initCalled{false};
    bool startCalled{false};
};

class TestPotmeterTask final : public IPotmeterTask {
   public:
    esp_err_t init() override {
        initCalled = true;
        return ESP_OK;
    }

    esp_err_t start() override {
        startCalled = true;
        return ESP_OK;
    }

    bool initCalled{false};
    bool startCalled{false};
};

}  // namespace

TEST(FunctionTaskInterfacesTest, ButtonTaskInterfaceSupportsLifecycle) {
    TestButtonTask task;
    IButtonTask &interfaceRef = task;

    EXPECT_EQ(ESP_OK, interfaceRef.init());
    EXPECT_EQ(ESP_OK, interfaceRef.start());
    EXPECT_TRUE(task.initCalled);
    EXPECT_TRUE(task.startCalled);
}

TEST(FunctionTaskInterfacesTest, GpioLedTaskInterfaceSupportsLifecycle) {
    TestGpioLedTask task;
    IGpioLedTask &interfaceRef = task;

    EXPECT_EQ(ESP_OK, interfaceRef.init());
    EXPECT_EQ(ESP_OK, interfaceRef.start());
    EXPECT_TRUE(task.initCalled);
    EXPECT_TRUE(task.startCalled);
}

TEST(FunctionTaskInterfacesTest, Max7219TaskInterfaceSupportsLifecycle) {
    TestMax7219Task task;
    IMax7219Task &interfaceRef = task;

    EXPECT_EQ(ESP_OK, interfaceRef.init());
    EXPECT_EQ(ESP_OK, interfaceRef.start());
    EXPECT_TRUE(task.initCalled);
    EXPECT_TRUE(task.startCalled);
}

TEST(FunctionTaskInterfacesTest, PotmeterTaskInterfaceSupportsLifecycle) {
    TestPotmeterTask task;
    IPotmeterTask &interfaceRef = task;

    EXPECT_EQ(ESP_OK, interfaceRef.init());
    EXPECT_EQ(ESP_OK, interfaceRef.start());
    EXPECT_TRUE(task.initCalled);
    EXPECT_TRUE(task.startCalled);
}