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
#include "enc_ver.h"

#ifdef VERIFIC_NAMESPACE
using namespace Verific;
#endif
using namespace std;
std::string readFile(const std::string &filePath)
{
    std::ifstream in_stream(filePath);
    if (!in_stream)
    {
        throw std::runtime_error("Cannot open file: " + filePath);
    }
    return std::string((std::istreambuf_iterator<char>(in_stream)), std::istreambuf_iterator<char>());
}

void writeFile(const std::string &filePath, const std::string &content)
{
    std::ofstream out_stream(filePath);
    if (!out_stream)
    {
        throw std::runtime_error("Cannot open file: " + filePath);
    }
    out_stream << content;
}

void appendDataAtStart(const std::string &input_file, const std::string &data_file, const std::string &output_file)
{
    std::string data_to_append = readFile(data_file);
    std::string original_content = readFile(input_file);

    std::string new_content = data_to_append + "\n" + "`pragma protect begin" + "\n" + original_content + "\n" + "`pragma protect end";

    writeFile(output_file, new_content);
}

bool isValidFileExtension(const std::string &file)
{
    std::string extension = std::filesystem::path(file).extension().string();
    return extension == ".v" || extension == ".sv";
}

std::string getOutputFileName(const std::string &input_file_name)
{
    std::filesystem::path input_path(input_file_name);
    std::string output_file_name = input_path.stem().string() + ".vp";
    return output_file_name;
}

int enc_ver(const char *file_name, const char *out_file_name)
{
    // Cipher object to plug in the encryption/decryption algorithm
    ieee_1735 ieee_1735;

    // Encrypt the file and write it to the output file
    if (!hdl_encrypt::EncryptVerilogFile(file_name, out_file_name, &ieee_1735))
    {
        std::cout << ">>> The file " << file_name << " cannot be encrypted." << std::endl;
        return 1;
    }

    return 0;
}
