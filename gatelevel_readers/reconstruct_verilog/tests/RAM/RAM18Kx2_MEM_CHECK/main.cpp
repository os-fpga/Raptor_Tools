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
#include "reconstruct_ram18kx2.h"

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
std::string extract_sram1(const std::string &init, const std::string &parity) {
  std::string sram(18432, '0');

  for (int i = 0; i < 1024; i++) {
    sram.replace(i * 18, 16, init.substr(i * 16, 16));
    sram.replace((i + 1) * 16 + (2 * i), 2, parity.substr(i * 2, 2));
  }

  return sram;
}

std::string extract_sram2(const std::string &init, const std::string &parity) {
  std::string sram(18432, '0');

  for (int i = 0; i < 1024; i++) {
    sram.replace(i * 18, 16, init.substr(i * 16, 16));
    sram.replace((i + 1) * 16 + (2 * i), 2, parity.substr(i * 2, 2));
  }

  return sram;
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
std::string get_init_i1(std::string &init1, std::string &parity1,
                        std::string &init2, std::string &parity2) {
  std::reverse(begin(init1), end(init1));
  std::reverse(begin(parity1), end(parity1));
  std::reverse(begin(init2), end(init2));
  std::reverse(begin(parity2), end(parity2));
  std::string res = extract_sram1(init1, parity1) + extract_sram2(init2, parity2);
  std::reverse(begin(init1), end(init1));
  std::reverse(begin(parity1), end(parity1));
  std::reverse(begin(init2), end(init2));
  std::reverse(begin(parity2), end(parity2));
  std::reverse(begin(res), end(res));
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

int main() {
  // int length;

  // // Get string length from user
  // std::cout << "Enter desired string length (positive integer): ";
  // std::cin >> length;

  // // Validate user input
  // if (length <= 0) {
  //   std::cerr << "Invalid input: Length must be positive." << std::endl;
  //   return 1;
  // }

  // Get output file name from user
  std::string filename("memData.txt");
  // std::cout << "Enter output file name: ";
  // std::cin >> filename;

  // Generate and save the random string
  std::string random_str = random_string(14);

  std::string init1_p = random_string(4 * 512);
  std::string init1 = random_string(4 * 4096);
  std::string init2_p = random_string(4 * 512);
  std::string init2 = random_string(4 * 4096);

  std::string initMem = TDP_RAM18KX2_instance::get_init_i1(init1, init1_p, init2, init2_p);
  std::string initMem_h =
      "   localparam [36864-1:0] INIT_i1_cpp = 36864'h" + stringEncode(initMem) + ";";
  std::string init1_p_h =
      "   parameter [2048-1:0] INIT1_PARITY = 2048'h" + stringEncode(init1_p) + ";";
  std::string init1_h =
      "   parameter [16384-1:0] INIT1 =  16384'h" + stringEncode(init1) + ";";
  std::string init2_p_h =
      "   parameter [2048-1:0] INIT2_PARITY = 2048'h" + stringEncode(init2_p) + ";";
  std::string init2_h =
      "   parameter [16384-1:0] INIT2 = 16384'h" + stringEncode(init2) + ";";

  std::ofstream outfile(filename);

  if (!outfile.is_open()) {
    std::cerr << "Error opening file: " << filename << std::endl;
    return 1;
  }

  // outfile << init1_p << std::endl;
  // outfile << init1 << std::endl;
  // outfile << init2_p << std::endl;
  // outfile << init2 << std::endl;

  outfile << init1_p_h << std::endl;
  outfile << init1_h << std::endl;
  outfile << init2_p_h << std::endl;
  outfile << init2_h << std::endl;
  outfile << initMem_h << std::endl;

  outfile.close();

  std::cout << "Initial random mem generated and saved to: " << filename
            << std::endl;

  return 0;
}
