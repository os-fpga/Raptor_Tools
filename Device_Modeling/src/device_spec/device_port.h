/**
 * @file device_port.h
 * @brief Defines the device_port class, which represents a port in a device.
 * @version 0.1
 * @date 2023-05-18
 * @author Manadher Kharroubi (manadher@rapidsilicon.com)
 *
 */

#pragma once
#include <sstream>
#include <vector>
#include <memory>
#include "device_pin.h"

class device_port
{
public:
    /**
     * @brief Constructor that initializes the device_port.
     * @param num_pins The number of pins in the port.
     * @param name The name of the block.
     * @param is_in The direction of the port.
     */
    device_port(unsigned int num_pins, std::string name, bool is_in)
        : name_(name), is_in_(is_in)
    {
        for (unsigned int i = 0; i < num_pins; ++i)
        {
            std::string pin_name = name + "__" + std::to_string(i) + "__";
            pin_vector_.push_back(new device_pin(pin_name, is_in));
        }
    }

    /**
     * @brief Copy constructor that creates a copy of another device_port object.
     * @param other The device_port object to be copied.
     */
    device_port(const device_port &other)
        : name_(other.get_name()), is_in_(other.is_input())
    {
        for (size_t i = 0; i < other.get_size(); ++i)
        {
            pin_vector_.push_back(new device_pin(device_pin(*get_pin(i))));
        }
    }

    /**
     * @brief Get the name of the device_port.
     * @return The name of the device_port.
     */
    const std::string &get_name() const
    {
        return name_;
    }

    /**
     * @brief Default constructor that creates a device_port with default values.
     */
    device_port()
        : name_("__default_name__"), is_in_(true)
    {
    }

    /**
     * @brief Get the pin at the specified index.
     * @param index The index of the pin.
     * @return The pin at the specified index.
     */
    device_pin* get_pin(unsigned int index)
    {
        if (index < pin_vector_.size())
        {
            return pin_vector_[index];
        }
        else
        {
            return nullptr;
        }
    }

    /**
     * @brief Get the direction of the device_port.
     * @return The direction of the device_port.
     */
    std::string direction() const { return is_in_ ? "in" : "out"; }

    /**
     * @brief Get the size (number of pins) of the device_port.
     * @return The size of the device_port.
     */
    size_t get_size() const
    {
        return pin_vector_.size();
    }

    /**
     * @brief Check if the device_port is an input port.
     * @return True if the device_port is an input port, false otherwise.
     */
    bool is_input() const { return is_in_; }

    /**
     * @brief Check if the device_port is an output port.
     * @return True if the device_port is an output port, false otherwise.
     */
    bool is_output() const { return !is_in_; }

    /**
     * @brief Convert the device_port to a string representation.
     * @return A string representation of the device_port.
         * The string representation includes the port name, direction, and pins.
     * The pins are enclosed in square brackets and separated by commas.
     * Example: "Port Name: port_name, Direction: in, Pins: [pin1, pin2, pin3]"
     */
    std::string to_string() const
    {
        std::ostringstream oss;
        oss << "Port Name: " << name_ << ", Direction: " << direction() << ", Pins: [";
        for (const auto &pin : pin_vector_)
        {
            oss << pin->to_string() << ", ";
        }
        std::string str = oss.str();
        str = str.substr(0, str.length() - 2); // remove the last comma and space
        str += "]";
        return str;
    }

    /**
     * @brief Equality comparison operator for device_port objects.
     * @param rhs The right-hand side device_port object.
     * @return True if the device_port objects are equal, false otherwise.
     */
    bool operator==(const device_port &rhs) const
    {
        return (this == &rhs);
    }

    /**
     * @brief Compare the device_port with another device_port for equality.
     * @param rhs The right-hand side device_port object.
     * @return True if the device_port objects are equal, false otherwise.
     */
    bool equal(const device_port &rhs) const
    {
        if (pin_vector_.size() != rhs.pin_vector_.size())
        {
            return false;
        }

        if (name_ != rhs.name_)
        {
            return false;
        }

        if (is_in_ != rhs.is_in_)
        {
            return false;
        }

        for (size_t i = 0; i < pin_vector_.size(); ++i)
        {
            if (pin_vector_[i]->equal(*rhs.pin_vector_[i]))
            {
                return false;
            }
        }

        return true;
    }

private:
    std::string name_;                                      ///< The name of the device_port
    bool is_in_;                                            ///< The direction of the device_port (true for input, false for output)
    std::vector<device_pin* > pin_vector_;   ///< The vector of pins in the device_port
};

/**
 * @brief Overload the << operator for device_port.
 * @param os The ostream.
 * @param port The device_port.
 * @return The ostream.
 */
inline std::ostream &operator<<(std::ostream &os, const device_port &port)
{
    os << port.to_string();
    return os;
}

/**
 * @brief Overload the << operator for device_port* .
 * @param os The ostream.
 * @param port The ptr to the device_port.
 * @return The ostream.
 */
inline std::ostream &operator<<(std::ostream &os, const device_port* port)
{
    if (port)
    {
        os << port->to_string();
    }
    else
    {
        os << "null";
    }
    return os;
}



