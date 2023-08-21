/**
 * @file device_net_test.cpp
 * @author Manadher Kharroubi (manadher@rapidsilicon.com)
 * @brief 
 * @version 0.1
 * @date 2023-05-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <gtest/gtest.h>
#include "device_net.h"
#include "logging_utilities.h"
#include <spdlog/sinks/null_sink.h>
#include <gmock/gmock.h>


using namespace ::testing;

// Test fixture for the device_net class
class DeviceNetTest : public Test
{
protected:
    void SetUp() override
    {
        net1 = new device_net("Net1", "Signal1");
        net2 = new device_net("Net2", "Signal2");
    }

    void TearDown() override
    {
        delete net1;
        delete net2;
    }

    device_net* net1;
    device_net* net2;
};

// Tests the get_net_name() function
TEST_F(DeviceNetTest, GetNetName)
{
    EXPECT_EQ(net1->get_net_name(), "Net1");
    EXPECT_EQ(net2->get_net_name(), "Net2");
}

// Tests the set_net_name() function
TEST_F(DeviceNetTest, SetNetName)
{
    net1->set_net_name("NewName");
    EXPECT_EQ(net1->get_net_name(), "NewName");
}

// Tests the get_signal_name() function
TEST_F(DeviceNetTest, GetSignalName)
{
    EXPECT_EQ(net1->get_signal_name(), "Signal1");
    EXPECT_EQ(net2->get_signal_name(), "Signal2");
}

// Tests the set_signal_name() function
TEST_F(DeviceNetTest, SetSignalName)
{
    net1->set_signal_name("NewSignal");
    EXPECT_EQ(net1->get_signal_name(), "NewSignal");
}

// Tests the get_source() and set_source() functions
TEST_F(DeviceNetTest, SourceNet)
{
    net1->set_source(net2);
    EXPECT_EQ(net1->get_source(), net2);
}

// Tests the get_sink_set() and add_sink() functions
TEST_F(DeviceNetTest, SinkNets)
{
    net1->add_sink(net2);
    EXPECT_THAT(net1->get_sink_set(), Contains(net2));
}

// Tests the equality operator (operator==)
TEST_F(DeviceNetTest, EqualityOperator)
{
    device_net net1_copy(*net1);
    EXPECT_FALSE(*net1 == net1_copy);
    EXPECT_FALSE(*net1 == *net2);
}

// Tests the inequality operator (operator!=)
TEST_F(DeviceNetTest, InequalityOperator)
{
    device_net net1_copy(*net1);
    EXPECT_TRUE(*net1 != net1_copy);
    EXPECT_TRUE(*net1 != *net2);
}

// Tests the to_string() function
TEST_F(DeviceNetTest, ToString)
{
    std::string expectedString = "Net Name: Net1\nSignal Name: Signal1\nSource: Net2\nSinks: ";
    EXPECT_EQ(net1->to_string(), expectedString);
}

// Tests the output stream operator (operator<<)
TEST_F(DeviceNetTest, OutputStreamOperator)
{
    std::stringstream ss;
    ss << *net1;
    std::string expectedString = "Net Name: Net1\nSignal Name: Signal1\nSource: null\nSinks: ";
    EXPECT_EQ(ss.str(), expectedString);
}

int main(int argc, char **argv)
{
    NULL_LOG;
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
    DEFAULT_LOG;
}
