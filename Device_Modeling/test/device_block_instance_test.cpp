#include <gtest/gtest.h>
#include "device_instance.h"

// Test fixture for device_block_instance
class DeviceBlockInstanceTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up any common resources or configurations for the tests
    }

    void TearDown() override {
        // Clean up any resources allocated in SetUp()
    }
};

// Test case for getters and setters
TEST_F(DeviceBlockInstanceTest, GettersAndSetters) {
    device_block_instance instance;

    // Test instance_name
    instance.set_instance_name("Instance1");
    EXPECT_EQ(instance.get_instance_name(), "Instance1");

    // Test io_bank
    instance.set_io_bank("Bank1");
    EXPECT_EQ(instance.get_io_bank(), "Bank1");

    // Test instance_id
    instance.set_instance_id(42);
    EXPECT_EQ(instance.get_instance_id(), 42);

    // Test logic_location_x
    instance.set_logic_location_x(10);
    EXPECT_EQ(instance.get_logic_location_x(), 10);

    // Test logic_location_y
    instance.set_logic_location_y(20);
    EXPECT_EQ(instance.get_logic_location_y(), 20);

    // Test logic_address
    instance.set_logic_address(0xABCD);
    EXPECT_EQ(instance.get_logic_address(), 0xABCD);
}

// Test case for operator<< overload
TEST_F(DeviceBlockInstanceTest, OutputOperator) {
    device_block_instance instance;
    instance.set_instance_name("Instance1");
    instance.set_io_bank("Bank1");
    instance.set_instance_id(42);
    instance.set_logic_location_x(10);
    instance.set_logic_location_y(20);
    instance.set_logic_address(0xABCD);

    std::stringstream ss;
    ss << instance;

    std::string expectedOutput =
        "Device block: __default_block_name__\n"
        "Ports:\nSignals:\nDouble parameters:\n"
        "Int parameters:\nString parameters:\n"
        "Attributes:\nInstances:\nConstraints:\n"
        "Instance Name: Instance1\nIO Bank: Bank1\n"
        "Instance ID: 42\nLogic Location X: 10\n"
        "Logic Location Y: 20\nLogic Address: 43981\n";

    EXPECT_EQ(ss.str(), expectedOutput);
}
