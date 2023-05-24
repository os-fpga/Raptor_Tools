/**
 * @file device.h
 * @brief Contains the definition of the device class, which represents a device_block with additional attributes.
 * @author Manadher Kharroubi (manadher@rapidsilicon.com)
 * @version 0.1
 * @date 2023-05-18
 * 
 * @details The device class is derived from the device_block class and adds additional attributes
 *          such as schema version, device name, and device version. It provides getters and setters
 *          for these attributes and inherits all the members of the base class.
 */

#pragma once

#include <unordered_map>
#include <string>
#include <iostream>

#include "device_block.h"
#include "rs_expression.h"
#include "spdlog/spdlog.h"

/**
 * @class device
 * @brief Represents a device with additional attributes.
 * @extends device_block
 */
class device : public device_block
{
public:
    /**
     * @brief Default constructor for the device class.
     */
    device() : device_name_("__default_device_name__") {
        block_type_ = "device";
    }

    /**
     * @brief Constructor for the device class with a specified device name.
     * @param device_name The name of the device.
     */
    device(const std::string& device_name) : device_name_(device_name) {
        block_type_ = "device";
    }

    /**
     * @brief Get the schema version of the device.
     * @return The schema version of the device.
     */
    const std::string& schema_version() const { return schema_version_; }

    /**
     * @brief Set the schema version of the device.
     * @param schema_version The schema version to set.
     */
    void set_schema_version(const std::string& schema_version) { schema_version_ = schema_version; }

    /**
     * @brief Get the name of the device.
     * @return The name of the device.
     */
    const std::string& device_name() const { return device_name_; }

    /**
     * @brief Set the name of the device.
     * @param device_name The name to set for the device.
     */
    void set_device_name(const std::string& device_name) { device_name_ = device_name; }

    /**
     * @brief Get the version of the device.
     * @return The version of the device.
     */
    const std::string& device_version() const { return device_version_; }

    /**
     * @brief Set the version of the device.
     * @param device_version The version to set for the device.
     */
    void set_device_version(const std::string& device_version) { device_version_ = device_version; }

    /**
     * @brief Overload of the operator << to print the device details.
     * @param os The output stream.
     * @param device The device to print.
     * @return The output stream after printing the device details.
     */
    friend std::ostream& operator<<(std::ostream& os, const device& device)
    {
        os << "Device:" << std::endl;
        os << "  Schema Version: " << device.schema_version_ << std::endl;
        os << "  Device Name: " << device.device_name_ << std::endl;
        os << "  Device Version: " << device.device_version_ << std::endl;
        os << static_cast<const device_block&>(device); // Print base class (device_block) details
        return os;
    }

private:
    std::string schema_version_; ///< The schema version of the device.
    std::string device_name_;    ///< The name of the device.
    std::string device_version_; ///< The version of the device.
};
