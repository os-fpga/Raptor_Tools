/**
 * @file device_signal_test.cpp
 * @author Manadher Kharroubi (manadher@rapidsilicon.com)
 * @brief 
 * @version 0.1
 * @date 2023-05-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <gtest/gtest.h>
#include "device_signal.h"
#include "logging_utilities.h"
#include <spdlog/sinks/null_sink.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "device_signal.h"

using namespace ::testing;

// Test fixture for the device_signal class
class DeviceSignalTest : public Test
{
protected:
    void SetUp() override
    {
        signal1 = new device_signal("Signal1", 3);
        signal2 = new device_signal("Signal2", 2);
    }

    void TearDown() override
    {
        delete signal1;
        delete signal2;
    }

    device_signal* signal1;
    device_signal* signal2;
};

// Tests the get_name() function
TEST_F(DeviceSignalTest, GetName)
{
    EXPECT_EQ(signal1->get_name(), "Signal1");
    EXPECT_EQ(signal2->get_name(), "Signal2");
}

// Tests the set_name() function
TEST_F(DeviceSignalTest, SetName)
{
    std::string newName = "NewSignal";
    signal1->set_name(newName);
    EXPECT_EQ(signal1->get_name(), newName);
}

// Tests the get_size() function
TEST_F(DeviceSignalTest, GetSize)
{
    EXPECT_EQ(signal1->get_size(), 3);
    EXPECT_EQ(signal2->get_size(), 2);
}

// Tests the get_net_vector() function
TEST_F(DeviceSignalTest, GetNetVector)
{
    const std::vector<device_net*>& netVector1 = signal1->get_net_vector();
    const std::vector<device_net*>& netVector2 = signal2->get_net_vector();
    EXPECT_EQ(netVector1.size(), 3);
    EXPECT_EQ(netVector2.size(), 2);
}

// Tests the replace_net() function
TEST_F(DeviceSignalTest, ReplaceNet)
{
    device_net* net = new device_net("NewNet", "Signal1");
    signal1->replace_net(1, net);
    EXPECT_EQ(signal1->get_net(1), net);
}

// Tests the equal() function
TEST_F(DeviceSignalTest, Equal)
{
    device_signal signal1_copy(*signal1);
    EXPECT_TRUE(signal1->equal(signal1_copy));
    EXPECT_FALSE(signal1->equal(*signal2));
}

// Tests the equality operator (operator==)
TEST_F(DeviceSignalTest, EqualityOperator)
{
    device_signal signal1_copy(*signal1);
    EXPECT_FALSE(*signal1 == signal1_copy);
    EXPECT_FALSE(*signal1 == *signal2);
}

// Tests the inequality operator (operator!=)
TEST_F(DeviceSignalTest, InequalityOperator)
{
    device_signal signal1_copy(*signal1);
    EXPECT_TRUE(*signal1 != signal1_copy);
    EXPECT_TRUE(*signal1 != *signal2);
}

// Tests the add_net() function
TEST_F(DeviceSignalTest, AddNet)
{
    device_net* net = new device_net("NewNet", "Signal1");
    signal1->add_net(net);
    EXPECT_EQ(signal1->get_net_vector().back(), net);
}

// Tests the get_net() function
TEST_F(DeviceSignalTest, GetNet)
{
    const std::vector<device_net*>& netVector = signal1->get_net_vector();
    EXPECT_EQ(signal1->get_net(0), netVector[0]);
    EXPECT_EQ(signal1->get_net(1), netVector[1]);
    EXPECT_EQ(signal1->get_net(2), netVector[2]);
}

// Tests the output stream operator (operator<<)
TEST_F(DeviceSignalTest, OutputStreamOperator)
{
    std::stringstream ss;
    ss << *signal1;
    std::string expectedString = "Signal Name: Signal1, Size: 3, "
                                "Nets: Net Name: Signal1__0__\nSignal Name: Signal1\n"
                                "Source: null\nSinks:  Net Name: Signal1__1__\n"
                                "Signal Name: Signal1\nSource: null\n"
                                "Sinks:  Net Name: Signal1__2__\n"
                                "Signal Name: Signal1\nSource: null\nSinks:  ";
    EXPECT_EQ(ss.str(), expectedString);
}

// TEST(DeviceSignalTest, ConstructorTest)
// {
//     device_signal signal("test_signal", 3);
//     EXPECT_EQ(signal.get_name(), "test_signal");
//     EXPECT_EQ(signal.get_size(), 3);
//     for (int i = 0; i < 3; ++i)
//     {
//         EXPECT_EQ(signal.get_net(i)->get_signal_name(), "test_signal");
//         EXPECT_EQ(signal.get_net(i)->get_net_name(), "test_signal__" + std::to_string(i) + "__");
//     }
// }

// TEST(DeviceSignalTest, AddNetTest)
// {
//     device_signal signal("test_signal", 3);
//     auto new_net = std::make_shared<device_net>("new_net", "test_signal");
//     signal.add_net(new_net);
//     EXPECT_EQ(signal.get_size(), 4);
//     EXPECT_EQ(signal.get_net(3), new_net);
// }

// TEST(DeviceSignalTest, ReplaceNetTest)
// {
//     device_signal signal("test_signal", 3);
//     auto new_net = std::make_shared<device_net>("new_net", "test_signal");
//     signal.replace_net(1, new_net);
//     EXPECT_EQ(signal.get_net(1), new_net);
// }

// TEST(DeviceSignalTest, EqualityOperatorTest)
// {
//     device_signal signal1("test_signal", 3);
//     device_signal signal2("test_signal", 3);
//     EXPECT_TRUE(signal1.equal(signal2));
//     device_signal signal3("test_signal", 4);
//     EXPECT_FALSE(signal1.equal(signal3));
//     device_signal signal4("another_signal", 3);
//     EXPECT_NE(signal1, signal4);
// }

// TEST(DeviceSignalTest, OstreamOperatorTest)
// {
//     device_signal signal("test_signal", 3);
//     std::ostringstream oss;
//     oss << signal;
//     EXPECT_EQ(oss.str(), "Signal Name: test_signal, Size: 3, Nets: Net Name: test_signal__0__\nSignal Name: test_signal\nSource: \nSinks:  Net Name: test_signal__1__\nSignal Name: test_signal\nSource: \nSinks:  Net Name: test_signal__2__\nSignal Name: test_signal\nSource: \nSinks:  ");
// }
