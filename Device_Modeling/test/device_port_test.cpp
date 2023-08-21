/**
 * @file device_port_test.cpp
 * @author Manadher Kharroubi (manadher@rapidsilicon.com)
 * @brief 
 * @version 0.1
 * @date 2023-05-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <gtest/gtest.h>
#include "device_pin.h"  // Assume device_pin and device_port are in these headers
#include "device_port.h"

TEST(DevicePinTest, GetName) {
    device_pin pin("pin1", true);
    EXPECT_EQ(pin.get_name(), "pin1");
}

TEST(DevicePinTest, IsInput) {
    device_pin pin("pin1", true);
    EXPECT_TRUE(pin.is_input());
}

TEST(DevicePinTest, IsOutput) {
    device_pin pin("pin1", false);
    EXPECT_TRUE(pin.is_output());
}

TEST(DevicePortTest, GetPin) {
    device_port port(5, "block1", true);
    auto pin = port.get_pin(3);
    EXPECT_NE(pin, nullptr);
    EXPECT_EQ(pin->get_name(), "block1__3__");
}

TEST(DevicePortTest, GetPinOutOfRange) {
    device_port port(5, "block1", true);
    auto pin = port.get_pin(10);
    EXPECT_EQ(pin, nullptr);
}

TEST(DevicePortTest, IsInput) {
    device_port port(5, "block1", true);
    EXPECT_TRUE(port.is_input());
}

TEST(DevicePortTest, IsOutput) {
    device_port port(5, "block1", false);
    EXPECT_TRUE(port.is_output());
}
