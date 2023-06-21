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

void displayWarningNoPragma(const std::string &file_name)
{
    std::cerr << "Warning: No `pragma ` and Public key found in file: " << file_name << std::endl;
}

int main(int argc, char **argv)
{
    try
    {
        std::vector<std::string> data_files;
        std::vector<std::string> input_files;
        std::vector<std::string> intermediate_files;
        std::string output_directory = ""; // Default is current directory

        bool help_flag = false;

        for (int i = 1; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (arg == "-h" || arg == "--help")
            {
                help_flag = true;
            }
            else if (arg == "-k" || arg == "--keyfile")
            {
                if (i + 1 < argc)
                {
                    ++i;
                    while (i < argc && argv[i][0] != '-')
                    {
                        data_files.push_back(argv[i]);
                        ++i;
                    }
                    --i;
                }
                else
                {
                    std::cerr << "--keyfile option requires at least one argument." << std::endl;
                    return 1;
                }
            }
            else if (arg == "-f" || arg == "--hdl-files")
            {
                if (i + 1 < argc)
                {
                    ++i;
                    while (i < argc && argv[i][0] != '-')
                    {
                        std::string file_name = argv[i];
                        if (isValidFileExtension(file_name))
                        {
                            input_files.push_back(file_name);
                        }
                        else
                        {
                            std::cerr << "Invalid input HDL file extension: " << file_name << " Only .v and .sv extensions are allowed." << std::endl;
                            return 1;
                        }
                        ++i;
                    }
                    --i;
                }
                else
                {
                    std::cerr << "--hdl-files option requires at least one argument." << std::endl;
                    return 1;
                }
            }
            else if (arg == "-o" || arg == "--output-dir")
            {
                if (i + 1 < argc)
                {
                    output_directory = argv[++i];
                }
                else
                {
                    std::cerr << "--output-dir option requires one argument." << std::endl;
                    return 1;
                }
            }
        }

        if (help_flag)
        {
            std::cout << "\n\t\t\t**********************************\n"
                      << "\t\t\t*  RapidSilicon Encryption Tool *\n"
                      << "\t\t\t**********************************\n"
                      << "Usage:\n"
                      << "\t-h | --help\t\tShow help\n"
                      << "\t-k | --keyfile\t\tFile(s) containing the key if not embedded in HDL file\n"
                      << "\t-f | --hdl-files\tSpace separated HDL files with extension .v or .sv\n"
                      << "\t-o | --output-dir\tOutput directory for encrypted files" << std::endl;
            return 0;
        }

        if (input_files.empty())
        {
            help_flag = true;
        }

        for (const auto &file_name : input_files)
        {
            if (fileContainsPragmaDataBlock(file_name))
            {
                std::cout << ">>> Skipping file " << file_name << " as it is already encrypted." << std::endl;
                continue;
            }

            std::string out_file_name_str = getOutputFileName(file_name);
            if (!output_directory.empty())
            {
                // Append output directory to the intermediate file path
                out_file_name_str = std::filesystem::path(output_directory) / out_file_name_str;
            }

            if (!data_files.empty() && !fileContainsPragma(file_name))
            {
                std::string base_name = std::filesystem::path(file_name).filename().string();
                std::string intermediate_file = base_name;
                appendDataAtStart(file_name, data_files, intermediate_file);
                std::cout << "\t The Encrypted file is: " << out_file_name_str << std::endl;
                enc_ver(intermediate_file.c_str(), out_file_name_str.c_str());
                // Add intermediate file to the list for deletion
                intermediate_files.push_back(intermediate_file);
            }
            else
            {
                if (!fileContainsPragma(file_name))
                {
                    displayWarningNoPragma(file_name);
                    continue;
                }
                else
                {
                    std::cout << "\t The Encrypted file is: " << out_file_name_str << std::endl;
                    enc_ver(file_name.c_str(), out_file_name_str.c_str());
                }
            }

            for (const auto &intermediate_file : intermediate_files)
            {
                std::filesystem::remove(intermediate_file);
            }
        }
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}