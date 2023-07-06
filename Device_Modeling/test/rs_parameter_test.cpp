/**
 * @file rs_parameter_test.cpp
 * @author Manadher Kharroubi (manadher@rapidsilicon.com)
 * @brief 
 * @version 0.1
 * @date 2023-05-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <gtest/gtest.h>
#include "rs_parameter.h"

TEST(ParameterTest, Constructor)
{
    ParameterType<int> type;
    Parameter<int> param("test", 5, type);

    EXPECT_EQ(param.get_name(), "test");
    EXPECT_EQ(param.get_value(), 5);
}

TEST(ParameterTest, SetValue)
{
    ParameterType<int> type;
    Parameter<int> param("test", 5, type);
    param.set_value(10);

    EXPECT_EQ(param.get_value(), 10);
}

TEST(ParameterTest, InvalidValue)
{
    ParameterType<int> type;
    type.set_lower_bound(0);
    type.set_upper_bound(10);
    EXPECT_THROW(Parameter<int>("test", -1, type), std::runtime_error);
}

TEST(ParameterTest, SetAddress)
{
    ParameterType<int> type;
    Parameter<int> param("test", 5, type);
    param.set_address(123);

    EXPECT_EQ(param.get_address(), 123);
}

TEST(ParameterTest, IntToString)
{
    ParameterType<int> type;
    Parameter<int> param("test", 5, type);

    EXPECT_EQ(param.to_string(), "Parameter test: 5 of type int");
}

TEST(ParameterTest, DoubleToString)
{
    ParameterType<double> type;
    Parameter<double> param("test", 5.5, type);

    EXPECT_EQ(param.to_string(), "Parameter test: 5.5 of type double");
}

TEST(ParameterTest, StringToString)
{
    ParameterType<std::string> type;
    Parameter<std::string> param("test", "Test", type);

    EXPECT_EQ(param.to_string(), "Parameter test: Test of type string");
}

TEST(ParameterTest, InvalidSetAddress)
{
    ParameterType<double> type;
    Parameter<double> param("test", 5.5, type);

    EXPECT_THROW(param.set_address(123), std::runtime_error);
}
