/**
 * @file device_block_instance.h
 * @author Manadher Kharroubi (manadher@rapidsilicon.com)
 * @brief Contains the device_block_instance class definition.
 * @version 0.1
 * @date 2023-05-18
 *
 * @copyright Copyright (c) 2023
*/

#pragma once

#include "device_block.h"
#include "spdlog/spdlog.h"

/**
 * @class device_block_instance
 * @brief A derived class from device_block that represents an instance of a device block.
 */
class device_block_instance : public device_block {
public:
    /**
     * @brief Default constructor.
     */
    device_block_instance() : device_block(), instance_name_("__default_instance_name__"){
        block_type_ = "instance";
    }

    /**
     * @brief Gets the instance name.
     * @return The instance name.
     */
    const std::string& get_instance_name() const { return instance_name_; }

    /**
     * @brief Gets the IO bank.
     * @return The IO bank.
     */
    const std::string& get_io_bank() const { return io_bank_; }

    /**
     * @brief Gets the instance ID.
     * @return The instance ID.
     */
    const unsigned int get_instance_id() const { return instance_id_; }

    /**
     * @brief Gets the logic location X.
     * @return The logic location X.
     */
    int get_logic_location_x() const { return logic_location_x_; }

    /**
     * @brief Gets the logic location Y.
     * @return The logic location Y.
     */
    int get_logic_location_y() const { return logic_location_y_; }

    /**
     * @brief Gets the logic address.
     * @return The logic address.
     */
    int get_logic_address() const { return logic_address_; }

    /**
     * @brief Sets the instance name.
     * @param name The instance name to set.
     */
    void set_instance_name(const std::string& name) { instance_name_ = name; }

    /**
     * @brief Sets the IO bank.
     * @param bank The IO bank to set.
     */
    void set_io_bank(const std::string& bank) { io_bank_ = bank; }

    /**
     * @brief Sets the instance ID.
     * @param id The instance ID to set.
     */
    void set_instance_id(unsigned int id) { instance_id_ = id; }

    /**
     * @brief Sets the logic location X.
     * @param x The logic location X to set.
     */
    void set_logic_location_x(int x) { logic_location_x_ = x; }

    /**
     * @brief Sets the logic location Y.
     * @param y The logic location Y to set.
     */
    void set_logic_location_y(int y) { logic_location_y_ = y; }

    /**
     * @brief Sets the logic address.
     * @param address The logic address to set.
     */
    void set_logic_address(int address) { logic_address_ = address; }

    /**
     * @brief Overrides the operator << to print a device_block_instance.
     * @param os The output stream to write to.
     * @param instance The device_block_instance to print.
     * @return The modified output stream.
     */
    friend std::ostream& operator<<(std::ostream& os, const device_block_instance& instance) {
        // Call the base class operator<<
        os << static_cast<const device_block&>(instance);

        // Print the additional attributes
        os << "Instance Name: " << instance.instance_name_ << std::endl;
        os << "IO Bank: " << instance.io_bank_ << std::endl;
        os << "Instance ID: " << instance.instance_id_ << std::endl;
        os << "Logic Location X: " << instance.logic_location_x_ << std::endl;
        os << "Logic Location Y: " << instance.logic_location_y_ << std::endl;
        os << "Logic Address: " << instance.logic_address_ << std::endl;

        return os;
    }


private:
    std::string instance_name_;
    std::string io_bank_;
    unsigned int instance_id_;
    int logic_location_x_;
    int logic_location_y_;
    int logic_address_;
};

// Logging
// std::shared_ptr<spdlog::logger> logger_ = spdlog::stdout_logger_mt("device_block_instance");

// Add any additional member functions or data members here as needed.
