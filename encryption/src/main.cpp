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

#include <cstring>          // for memset
#include <iostream>

#include "enc_ver.h"

int main(int argc, char **argv)
{
 if (argc < 2)
    {
        std::cout << "Usage: program_name <input_file1> [<input_file2> ...]" << std::endl;
        return 1;
    }

    std::vector<std::string> input_files;

    // Iterate through the command-line arguments, skipping the program name
    for (int i = 1; i < argc; ++i)
    {
        const std::string file_name = argv[i]; // Set the input file name

        // Validate input file extension
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
        const char* out_file_name = out_file_name_str.c_str();

        std::cout << ">>> Encrypting file " << file_name << " into " << out_file_name << std::endl;
        enc_ver(file_name.c_str(), out_file_name);

    }

    return 0 ;
}
