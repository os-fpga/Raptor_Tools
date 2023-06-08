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

#include "hdl_encrypt.h"    // Make HDL encryptor available
#include "veri_file.h"      // Make Verilog reader available

#include "Strings.h"        // String utilities
#include "Protect.h"        // Make base protection class available to include encryption algorithm

#include "ieee_1735.h"
#include "enc_ver.h"

#ifdef VERIFIC_NAMESPACE
using namespace Verific ;
#endif

int enc_ver(const char *file_name, char *out_file_name)
{
    // Cipher object to plug in the encryption/decryption algorithm
    ieee_1735 ieee_1735 ;

    std::cout << ">>> Encrypting file " << file_name << " into " << out_file_name << std::endl ;

    // Encrypt the file and write it to the file out_file_name
    if (!hdl_encrypt::EncryptVerilogFile(file_name, out_file_name, &ieee_1735)) {
        std::cout << ">>> The file " << file_name << " cannot be encrypted." << std::endl ;
        Strings::free(out_file_name) ;
        return 1 ;
    }

    return 0 ;
}
