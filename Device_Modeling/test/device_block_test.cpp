/**
 * @file device_block_test.cpp
 * @author Manadher Kharroubi (manadher@rapidsilicon.com)
 * @brief
 * @version 0.1
 * @date 2023-05-18
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <gtest/gtest.h>

#include "device_block.h"

TEST(device_block, test_constructor)
{
    device_block block;
    EXPECT_EQ(block.ports().size(), 0);
    EXPECT_EQ(block.signals().size(), 0);
    EXPECT_EQ(block.double_parameters().size(), 0);
    EXPECT_EQ(block.int_parameters().size(), 0);
    EXPECT_EQ(block.string_parameters().size(), 0);
    EXPECT_EQ(block.attributes().size(), 0);
    EXPECT_EQ(block.instance_map().size(), 0);
    EXPECT_EQ(block.constraint_map().size(), 0);
}

TEST(device_block, test_getters_and_setters)
{
    device_block block;
    block.ports()["port_a"] = device_port(3, "port_a", true);
    block.signals()["signal_a"] = device_signal("signal_a", 5);
    block.double_parameters()["ParD"] = Parameter<double>("ParD", 5.1, ParameterType<double>());
    block.int_parameters()["ParI"] = Parameter<int>("ParI", 1, ParameterType<int>());
    block.string_parameters()["ParS"] = Parameter<string>("ParS", "string_parameter_a", ParameterType<string>());
    block.attributes()["Attr"] = Parameter<int>("Attr", 1, ParameterType<int>());
    // block.instance_map()["instance_a"] = shared_ptr<device_block_instance>(new device_block_instance("instance_a"));
    block.constraint_map()["constraint_a"] = rs_expression<int>("a >= 3");

    EXPECT_TRUE((block.ports()["port_a"]).equal(device_port(3, "port_a", true)));
    EXPECT_TRUE(block.signals()["signal_a"].equal(device_signal("signal_a", 5)));
    EXPECT_TRUE(block.double_parameters()["ParD"].equal(Parameter<double>("ParD", 5.1, ParameterType<double>())));
    EXPECT_TRUE(block.int_parameters()["ParI"].equal(Parameter<int>("ParI", 1, ParameterType<int>())));
    EXPECT_TRUE(block.string_parameters()["ParS"].equal(Parameter<string>("ParS", "string_parameter_a", ParameterType<string>())));
    EXPECT_TRUE(block.attributes()["Attr"].equal(Parameter<int>("Attr", 1, ParameterType<int>())));
    // EXPECT_EQ(block.instance_map()["instance_a"], shared_ptr<device_block_instance>(new device_block_instance("instance_a")));
    EXPECT_EQ(block.constraint_map()["constraint_a"], rs_expression<int>("a >= 3"));
}

TEST(device_block, test_operator_print)
{
    device_block block;
    block.ports()["port_a"] = device_port(3, "port_a", true);
    block.signals()["signal_a"] = device_signal("signal_a", 5);
    block.double_parameters()["double_parameter_a"] = Parameter<double>("double_parameter_a", 5.1, ParameterType<double>());
    block.int_parameters()["int_parameter_a"] = Parameter<int>("int_parameter_a", 1, ParameterType<int>());
    block.string_parameters()["string_parameter_a"] = Parameter<string>("string_parameter_a", "string_parameter_a", ParameterType<string>());
    block.attributes()["attribute_a"] = Parameter<int>("attribute_a", 1, ParameterType<int>());
    // block.instance_map()["instance_a"] = shared_ptr<device_block_instance>(new device_block_instance("instance_a"));
    block.constraint_map()["constraint_a"] = rs_expression<int>("a >= 3");

    std::stringstream ss;
    ss << block;
    EXPECT_EQ(ss.str(), "Device block: __default_block_name__\nPorts:\n"
                        "  port_a = Port Name: port_a, Direction: in, "
                        "Pins: [Pin Name: port_a__0__, Direction: in, "
                        "Pin Name: port_a__1__, Direction: in, "
                        "Pin Name: port_a__2__, Direction: in]\nSignals:\n"
                        "  signal_a = Signal Name: signal_a, Size: 5, "
                        "Nets: Net Name: signal_a__0__\nSignal Name: signal_a\n"
                        "Source: null\nSinks:  Net Name: signal_a__1__\n"
                        "Signal Name: signal_a\nSource: null\n"
                        "Sinks:  Net Name: signal_a__2__\nSignal Name: signal_a\n"
                        "Source: null\nSinks:  Net Name: signal_a__3__\n"
                        "Signal Name: signal_a\nSource: null\n"
                        "Sinks:  Net Name: signal_a__4__\nSignal Name: signal_a\n"
                        "Source: null\nSinks:  \nDouble parameters:\n  "
                        "double_parameter_a = Parameter double_parameter_a: 5.1 of type double\n"
                        "Int parameters:\n  int_parameter_a = Parameter int_parameter_a: 1 of type int\n"
                        "String parameters:\n  string_parameter_a = Parameter "
                        "string_parameter_a: string_parameter_a of type string\nAttributes:\n"
                        "  attribute_a = Parameter attribute_a: 1 of type int\n"
                        "Instances:\nConstraints:\n  constraint_a = Expression: a >= 3 (Not Evaluated)\n");
}
