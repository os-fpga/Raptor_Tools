#ifndef ENC_VER
#define ENC_VER
#include <vector>
#include <filesystem>
#include <cstring>          // for memset
#include <iostream>

#include "hdl_encrypt.h"    // Make HDL encryptor available
#include "veri_file.h"      // Make Verilog reader available

#include "Strings.h"        // String utilities
#include "Protect.h"        // Make base protection class available to include encryption algorithm

#include "ieee_1735.h"
int enc_ver(const char *file_name,const char *out_file_name );
bool isValidFileExtension(const std::string &file);
std::string getOutputFileName(const std::string &input_file_name);
std::vector<std::string> getFilesWithExtension(const std::string &directory, const std::string &extension);
#endif