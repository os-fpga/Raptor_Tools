/**
 * @file device_block.h
 * @author Manadher Kharroubi (manadher@rapidsilicon.com)
 * @brief 
 * @version 0.1
 * @date 2023-05-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include <unordered_map>
#include <string>
#include <iostream>

#include "device_port.h"
#include "device_signal.h"
#include "rs_parameter.h"
#include "rs_expression.h"
#include "spdlog/spdlog.h"

using namespace std;
class device_block_instance ;
class device_block
{
public:
    device_block()=default;
    device_block(const std::string& block_name) : block_name_(block_name) {}

    // Getters and setters
    const std::string& block_name()                                               const { return block_name_; }
    const std::string& block_type()                                               const { return block_type_; }
    unordered_map<string, device_port> &ports()                                         { return ports_map_; }
    const unordered_map<string, device_port> &ports()                             const { return ports_map_; }
    unordered_map<string, device_net> &nets()                                           { return nets_map_; }
    const unordered_map<string, device_net> &nets()                               const { return nets_map_; }
    unordered_map<string, device_signal> &signals()                                     { return signals_map_; }
    const unordered_map<string, device_signal> &signals()                         const { return signals_map_; }
    unordered_map<string, Parameter<double>> &double_parameters()                       { return double_parameters_map_; }
    const unordered_map<string, Parameter<double>> &double_parameters()           const { return double_parameters_map_; }
    unordered_map<string, Parameter<int>> &int_parameters()                             { return int_parameters_map_; }
    const unordered_map<string, Parameter<int>> &int_parameters()                 const { return int_parameters_map_; }
    unordered_map<string, Parameter<string>> &string_parameters()                       { return string_parameters_map_; }
    const unordered_map<string, Parameter<string>> &string_parameters()           const { return string_parameters_map_; }
    unordered_map<string, Parameter<int>> &attributes()                                 { return attributes_map_; }
    const unordered_map<string, Parameter<int>> &attributes()                     const { return attributes_map_; }
    unordered_map<string, device_block_instance*> &instance_map()            { return instance_map_; }
    const unordered_map<string, device_block_instance*> &instance_map()const { return instance_map_; }
    unordered_map<string, rs_expression<int>> &constraint_map()                         { return constraint_map_; }
    const unordered_map<string, rs_expression<int>> &constraint_map()             const { return constraint_map_; }

    // Overload of the operator <<
    friend ostream &operator<<(ostream &os, const device_block &block)
    {
        os << "Device block: " << block.block_name() << endl;
        os << "Ports:" << endl;
        for (const auto &port : block.ports())
        {
            os << "  " << port.first << " = " << port.second << endl;
        }
        os << "Signals:" << endl;
        for (const auto &signal : block.signals())
        {
            os << "  " << signal.first << " = " << signal.second << endl;
        }
        os << "Double parameters:" << endl;
        for (const auto &parameter : block.double_parameters())
        {
            os << "  " << parameter.first << " = " << parameter.second << endl;
        }
        os << "Int parameters:" << endl;
        for (const auto &parameter : block.int_parameters())
        {
            os << "  " << parameter.first << " = " << parameter.second << endl;
        }
        os << "String parameters:" << endl;
        for (const auto &parameter : block.string_parameters())
        {
            os << "  " << parameter.first << " = " << parameter.second << endl;
        }
        os << "Attributes:" << endl;
        for (const auto &parameter : block.attributes())
        {
            os << "  " << parameter.first << " = " << parameter.second << endl;
        }
        os << "Instances:" << endl;
        for (const auto &instance : block.instance_map())
        {
            os << "  " << instance.first << " = " << instance.second << endl;
        }
        os << "Constraints:" << endl;
        for (const auto &constraint : block.constraint_map())
        {
            os << "  " << constraint.first << " = " << constraint.second << endl;
        }
        return os;
    }

protected:
    std::string                             block_name_ = "__default_block_name__";
    std::string                             block_type_ = "block";
    unordered_map<string, device_port>      ports_map_;
    unordered_map<string, device_signal>    signals_map_;
    unordered_map<string, device_net>       nets_map_;
    unordered_map<string, Parameter<double>>double_parameters_map_;
    unordered_map<string, Parameter<int>>   int_parameters_map_;
    unordered_map<string, Parameter<string>>string_parameters_map_;
    unordered_map<string, Parameter<int>>   attributes_map_;
    unordered_map<string, device_block_instance*> instance_map_;
    unordered_map<string, rs_expression<int>> constraint_map_;

    friend class device_block_factory;
};
