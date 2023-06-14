/*
 *
 * (c) Copyright 1999 - 2022 Verific Design Automation Inc.
 * All rights reserved.
 *
 * This source code belongs to Verific Design Automation Inc.
 * It is considered trade secret and confidential, and is not to be used
 * by parties who have not received written authorization
 * from Verific Design Automation Inc.
 *
 * Only authorized users are allowed to use, copy and modify
 * this software provided that the above copyright notice
 * remains in all copies of this software.
 *
 *
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "enc_ver.h"
using namespace std;
int main(int argc, char **argv)
{
    if (argc < 3)
    {
        std::cout << "Usage: program_name <data_file> <input_file1> [<input_file2> ...]" << std::endl;
        return 1;
    }

    try
    {
        std::string data_file = argv[1]; // Data to be appended, can be any extension
        std::vector<std::string> input_files;

        for (int i = 2; i < argc; ++i)
        {
            const std::string file_name = argv[i]; // input files

            // Only check for .v and .sv extensions for input files
            if (!isValidFileExtension(file_name))
            {
                std::cout << ">>> Invalid input file or file extension: " << file_name << "\n NOTE: Only .v and .sv extensions are allowed." << std::endl;
                return 1;
            }

            input_files.push_back(file_name);
        }

        for (const auto &file_name : input_files)
        {
            std::string out_file_name_str = getOutputFileName(file_name);
            std::string base_name = std::filesystem::path(file_name).filename().string(); // Extract base name from file path
            std::string intermediate_file = base_name;

            appendDataAtStart(file_name, data_file, intermediate_file);

            std::cout << ">>> Encrypting file " << intermediate_file << " into " << out_file_name_str << std::endl;
            enc_ver(intermediate_file.c_str(), out_file_name_str.c_str());
        }
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
