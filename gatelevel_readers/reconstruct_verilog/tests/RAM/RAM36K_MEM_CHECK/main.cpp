/**
 * @file main.cpp
 * @author Manadher Kharroubi (manadher@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-02-20
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <random>
#include <string>
#include <map>
#include "reconstruct_ram36k.h"
using namespace std;

std::string random_string(int length) {
  // Check if the input length is positive
  if (length <= 0) {
    throw std::invalid_argument("Input length must be positive.");
  }

  // Initialize a random number generator
  std::random_device rd;
  std::mt19937 gen(rd());

  // Create a uniform distribution for generating 0 or 1
  std::uniform_int_distribution<int> distribution(0, 1);

  // Generate the random string
  std::string result;
  for (int i = 0; i < length; ++i) {
    result += std::to_string(distribution(gen));
  }

  return result;
}




std::map<char, string> hexDecoder = {
    {'0', "0000"}, {'1', "0001"}, {'2', "0010"}, {'3', "0011"}, {'4', "0100"},
    {'5', "0101"}, {'6', "0110"}, {'7', "0111"}, {'8', "1000"}, {'9', "1001"},
    {'A', "1010"}, {'B', "1011"}, {'C', "1100"}, {'D', "1101"}, {'E', "1110"},
    {'F', "1111"}, {'X', "xxxx"}, {'a', "1010"}, {'b', "1011"}, {'c', "1100"},
    {'d', "1101"}, {'e', "1110"}, {'f', "1111"}, {'x', "xxxx"}};

string bitsOfHexaDecimal(string &s) {
  if (!s.size()) {
    throw(std::invalid_argument(
        "Can't generate an hexadecimal number out of an empty string"));
  }
  for (auto &d : s) {
    if (d == 'z' || d == 'Z')
      d = 'x';
    if (!isxdigit(d) && d != 'x' && d != 'X')
      throw(std::invalid_argument("Non hexadigit in hexadecimal string" + s));
  }
  string res = "";
  res.reserve(4 * s.size());
  for (auto d : s) {
    for (auto c : hexDecoder[d])
      res.push_back(c);
  }
  return res;
}


char hexEncode(std::string val) {
  static std::unordered_map<string, char> hexEncoder = {
      {"0000", '0'}, {"0001", '1'}, {"0010", '2'}, {"0011", '3'},
      {"0100", '4'}, {"0101", '5'}, {"0110", '6'}, {"0111", '7'},
      {"1000", '8'}, {"1001", '9'}, {"1010", 'A'}, {"1011", 'B'},
      {"1100", 'C'}, {"1101", 'D'}, {"1110", 'E'}, {"1111", 'F'}};
  if (hexEncoder.find(val) == end(hexEncoder))
    return 'X';
  return hexEncoder[val];
}

std::string stringEncode(std::string &mval) {
  std::string res;
  for (int i = 0; i < mval.size(); i += 4) {
    res += hexEncode(mval.substr(i, 4));
  }
  return res;
}
  static std::string extract_sram1(const std::string &init,
                            const std::string &init_parity) {
    std::string sram1(18432, '0');
    for (int i = 0; i < 2048; i += 2) {
      sram1.replace((i * 9), 16, init.substr(i * 16, 16));
      sram1.replace(((i + 2) * 9) - 2, 2, init_parity.substr(i * 2, 2));
    }
    return sram1;
  }
  static std::string extract_sram2(const std::string &init,
                            const std::string &init_parity) {
    std::string sram2(18432, '0');
    for (int i = 1; i < 2048; i += 2) {
      sram2.replace((i - 1) * 9, 16, init.substr(i * 16, 16));
      sram2.replace(((i + 1) * 9) - 2, 2, init_parity.substr(i * 2, 2));
    }
    return sram2;
  }

  static std::string get_init_i1(std::string &init,
                          std::string &init_parity) {
    std::reverse(begin(init), end(init));
    std::reverse(begin(init_parity), end(init_parity));                 
    std::string res =  extract_sram1(init, init_parity) + extract_sram2(init, init_parity);
    std::reverse(begin(init), end(init));
    std::reverse(begin(init_parity), end(init_parity));  
    std::reverse(begin(res), end(res));
    return res;
  }
int main() {
  // Get output file name from user
  std::string filename("memData.txt");
  std::string init_p = random_string(4 * 1024);
  std::string init = random_string(4 * 8192);
  std::string initMem = TDP_RAM36K_instance::get_init_i1(init, init_p);
  std::string initMem_h =
      "   localparam [36864-1:0] INIT_i1_cpp = 36864'h" + stringEncode(initMem) + ";";
  std::string init_p_h =
      "   parameter [4096-1:0] INIT_PARITY = 4096'h" + stringEncode(init_p) + ";";
  std::string init_h =
      "   parameter [32768-1:0] INIT =  32768'h" + stringEncode(init) + ";";

  std::ofstream outfile(filename);

  if (!outfile.is_open()) {
    std::cerr << "Error opening file: " << filename << std::endl;
    return 1;
  }

  outfile << init_p_h << std::endl;
  outfile << init_h << std::endl;
  outfile << initMem_h << std::endl;

  outfile.close();

  std::cout << "Initial random mem generated and saved to: " << filename
            << std::endl;

  return 0;
}
