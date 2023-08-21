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
    // Get the input file name
   if (argc < 3)
    {
        return 4;
    }
    const char *file_name = argv[1]; // Set the file name as specified by the user
    char *out_file_name = argv[2];

    
    enc_ver(file_name, out_file_name);

    return 0 ;
}
