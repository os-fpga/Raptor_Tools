#include <gtest/gtest.h>
#include "device.h"

TEST(DeviceTest, SchemaVersion)
{
    device dev;
    dev.set_schema_version("1.0");
    EXPECT_EQ(dev.schema_version(), "1.0");
}

TEST(DeviceTest, DeviceName)
{
    device dev;
    dev.set_device_name("MyDevice");
    EXPECT_EQ(dev.device_name(), "MyDevice");
}

TEST(DeviceTest, DeviceVersion)
{
    device dev;
    dev.set_device_version("2.0");
    EXPECT_EQ(dev.device_version(), "2.0");
    EXPECT_EQ(dev.block_type(), "device");
}

TEST(DeviceTest, DeviceDetails)
{
    device dev("MyDevice");
    dev.set_schema_version("1.0");
    dev.set_device_version("2.0");

    std::ostringstream oss;
    oss << dev;

    std::string expectedOutput =
        "Device:\n"
        "  Schema Version: 1.0\n"
        "  Device Name: MyDevice\n"
        "  Device Version: 2.0\n"
        "Device block: __default_block_name__\n"
        "Ports:\n"
        "Signals:\n"
        "Double parameters:\n"
        "Int parameters:\n"
        "String parameters:\n"
        "Attributes:\n"
        "Instances:\n"
        "Constraints:\n";

    EXPECT_EQ(oss.str(), expectedOutput);
}
