/**
 * @file device_signal.h
 * @brief Class representing a signal in a device.
 * @author Manadher Kharroubi (manadher@rapidsilicon.com)
 * @version 0.1
 * @date 2023-05-18
 * @copyright Copyright (c) 2023
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include "device_net.h"
#include "spdlog/spdlog.h"
#include <iostream>

class device_block;

/**
 * @class device_signal
 * @brief Class representing a signal in a device.
 *
 * A signal is a collection of device_nets.
 */
class device_signal
{
public:
    /**
     * @brief Constructor that initializes the device_signal.
     * @param name The name of the signal.
     * @param size The size of the signal.
     */
    device_signal(const std::string &name, unsigned int size)
        : name_(name)
    {
        for (unsigned int i = 0; i < size; ++i)
        {
            std::ostringstream oss;
            oss << name << "__" << i << "__";
            net_vector_.push_back(new device_net(oss.str(), name));
        }
    }

    device_signal(const device_signal& other){
        name_ = other.get_name();
        for(const auto& net_ptr : other.get_net_vector()){
            add_net(new device_net(*net_ptr));
        }
    }
    
    /**
     * @brief Default constructor.
     */
    device_signal()
        : name_("__default_signal_name__")
    {
    }
    
    /**
     * @brief Set the name of the signal.
     * @param name The name of the signal.
     */
    void set_name(std::string &name)
    {
        name_ = name;
    }
    
    /**
     * @brief Get the name of the signal.
     * @return The name of the signal.
     */
    std::string get_name() const { return name_; }

    /**
     * @brief Get the size of the signal.
     * @return The size of the signal.
     */
    unsigned int get_size() const { return net_vector_.size(); }

    /**
     * @brief Get the vector of device_nets in the signal.
     * @return The vector of device_nets.
     */
    const std::vector< device_net* > &get_net_vector() const { return net_vector_; }

    /**
     * @brief Replace a net in the signal at the specified index.
     * @param index The index of the net to replace.
     * @param net The new net.
     */
    void replace_net(unsigned int index, device_net* net)
    {
        if (index < get_size())
        {
            net_vector_[index] = net;
        }
        else
        {
            spdlog::error("Index out of range in replace_net");
        }
    }
    
    /**
     * @brief Check if the device_signal is equal to another device_signal.
     * @param other The other device_signal to compare with.
     * @return True if the device_signals are equal, false otherwise.
     */
    bool equal(const device_signal &other) const
    {
        if (name_ != other.name_ || get_size() != other.get_size())
            return false;
        for (unsigned int idx = 0; idx < get_size(); ++idx)
        {
            if (!get_net(idx)->equal(*other.get_net(idx)))
                return false;
        }
        return true;
    }

    /**
     * @brief Equality comparison operator for device_signal objects.
     * @param other The other device_signal to compare with.
     * @return True if the device_signal objects are equal, false otherwise.
     */
    bool operator==(const device_signal &other) const
    {
        return (this == &other);
    }

    /**
     * @brief Inequality comparison operator for device_signal objects.
     * @param other The other device_signal to compare with.
     * @return True if the device_signal objects are not equal, false otherwise.
     */
    bool operator!=(const device_signal &other) const
    {
        return !(*this == other);
    }

    /**
     * @brief Add a net to the signal.
     * @param net The new net to add.
     */
    void add_net(device_net* net)
    {
        net_vector_.push_back(net);
    }
    
    /**
     * @brief Get the net at the specified index.
     * @param index The index of the net.
     * @return The net at the specified index.
     */
    device_net* get_net(unsigned int index) const
    {
        if (index < get_size())
        {
            return net_vector_[index];
        }
        else
        {
            spdlog::error("Index out of range in replace_net");
            return nullptr;
        }
    }

    /**
     * @brief Overload of the stream insertion operator for device_signal.
     * @param os The output stream.
     * @param signal The device_signal to output.
     * @return The updated output stream.
     */
    friend std::ostream& operator<<(std::ostream& os, const device_signal& signal)
    {
        os << "Signal Name: " << signal.name_ << ", Size: " << signal.get_size() << ", Nets: ";
        for (const auto& net : signal.net_vector_)
        {
            os << *net << " ";
        }
        return os;
    }

private:
    std::string name_;
    std::vector<device_net*> net_vector_;
};

