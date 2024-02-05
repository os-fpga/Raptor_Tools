#pragma once
/**
 * @file reconstruct_utils.h
 * @author Manadher Kharroubi (manadher@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-01-
 *
 * @copyright Copyright (c) 2024
 */
#include "boost/multiprecision/cpp_int.hpp"
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <regex>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace boost::multiprecision;
using namespace std;
std::vector<std::string> split_on_space(const std::string &line) {
  std::vector<std::string> tokens;
  std::stringstream ss(line);
  std::string token;
  while (ss >> token) {
    tokens.push_back(token);
  }
  return tokens;
}
unsigned uint_from_bits(std::string bits) {
  if (!bits.size() || bits.size() > 31)
    throw std::invalid_argument("Invalid 32 bit binary number ");
  unsigned pw = 1, res = 0;
  for (int i = bits.size() - 1; i + 1; --i) {
    if (bits[i] == '1')
      res += pw;
    else if (bits[i] != '0')
      throw std::invalid_argument("Invalid character in a binary number : " +
                                  std::string(1, bits[i]));
    pw <<= 1;
  }
  return res;
};