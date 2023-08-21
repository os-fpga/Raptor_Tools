#include "compare_ports.h"
#include <iostream>
#include <string>
#include <cstring>

int main(int argc, char** argv) {
    const char* rtlPortInfo = nullptr;
    const char* nlPortInfo = nullptr;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--rtlPortInfo") == 0 || strcmp(argv[i], "-rpi") == 0) {
            if (i + 1 < argc) {
                rtlPortInfo = argv[i + 1];
                ++i;
            } else {
                std::cerr << "Missing argument for --rtlPortInfo" << std::endl;
                return 1;
            }
        } else if (strcmp(argv[i], "--nlPortInfo") == 0 || strcmp(argv[i], "-npi") == 0) {
            if (i + 1 < argc) {
                nlPortInfo = argv[i + 1];
                ++i;
            } else {
                std::cerr << "Missing argument for --nlPortInfo" << std::endl;
                return 1;
            }
        } else {
            std::cerr << "Unknown argument: " << argv[i] << std::endl;
            return 1;
        }
    }

    if (rtlPortInfo == nullptr || nlPortInfo == nullptr) {
        std::cerr << "Missing required arguments." << std::endl;
        return 1;
    }

    compare_ports(rtlPortInfo, nlPortInfo);
    return 0;
}