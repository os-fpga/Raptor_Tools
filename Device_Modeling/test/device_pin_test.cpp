/**
 * @file device_pin_test.cpp
 * @author Manadher Kharroubi (manadher@rapidsilicon.com)
 * @brief 
 * @version 0.1
 * @date 2023-05-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <gtest/gtest.h>
#include "device_pin.h"
#include "device_net.h"

TEST(DevicePinTest, Constructor) {
    device_pin pin("pin1", true);
    EXPECT_EQ(pin.get_name(), "pin1");
    EXPECT_TRUE(pin.is_input());
    EXPECT_FALSE(pin.is_output());
    EXPECT_EQ(pin.direction(), "in");
    EXPECT_EQ(pin.get_sink_net()->get_net_name(), "pin1");
}

TEST(DevicePinTest, SetDrivingNet) {
    device_pin pin("pin1", true);
    auto net = new device_net("net1", "source1");
    pin.set_source_net(net);
    EXPECT_EQ(pin.get_sink_net()->get_source(), net);
    EXPECT_TRUE(net->get_sink_set().count(pin.get_sink_net()) > 0);
}

TEST(DevicePinTest, ToString) {
    // Create a device_pin object
    device_pin pin("test_pin", true);

    // Call the to_string() function
    std::string result = pin.to_string();

    // Check the result
    EXPECT_EQ(result, "Pin Name: test_pin, Direction: in");
}
