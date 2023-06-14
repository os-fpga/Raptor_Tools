#ifndef ENC_VER
#define ENC_VER
#include <vector>
#include <filesystem>
#include <cstring> // for memset
#include <iostream>
#include <fstream>
#include "hdl_encrypt.h" // Make HDL encryptor available
#include "veri_file.h"   // Make Verilog reader available

#include "Strings.h" // String utilities
#include "Protect.h" // Make base protection class available to include encryption algorithm

#include "ieee_1735.h"

int enc_ver(const char *file_name, const char *out_file_name);
std::string readFile(const std::string &filePath);
void writeFile(const std::string &filePath, const std::string &content);
void appendDataAtStart(const std::string &input_file, const std::string &data_file, const std::string &output_file);
bool isValidFileExtension(const std::string &file);
std::string getOutputFileName(const std::string &input_file_name);
std::vector<std::string> getFilesWithExtension(const std::string &directory, const std::string &extension);
#endif