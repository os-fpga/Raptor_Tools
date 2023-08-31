#include "compare_ports.h"
#include <iostream>
#include <string>
#include <cstring>

void display_help(const char* program_name) {
    std::cout << "Usage: " << program_name << " [--rtlPortInfo <rtlPort>] [--nlPortInfo <nlPort>] [-h|--help]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  --rtlPortInfo, -rpi <rtlPort> : Specify json containing RTL port information" << std::endl;
    std::cout << "  --nlPortInfo, -npi <nlPort>   : Specify json containing NL port information" << std::endl;
    std::cout << "  -h, --help                    : Show help message" << std::endl;
}

int main(int argc, char** argv) {
    const char* programName = argv[0];

    const char* rtlPortInfo = nullptr;
    const char* nlPortInfo = nullptr;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            display_help(programName);
            return 0;
        } else if (strcmp(argv[i], "--rtlPortInfo") == 0 || strcmp(argv[i], "-rpi") == 0) {
            if (i + 1 < argc) {
                rtlPortInfo = argv[i + 1];
                ++i;
            } else {
                std::cerr << "Error: Missing argument for --rtlPortInfo" << std::endl;
                return 1;
            }
        } else if (strcmp(argv[i], "--nlPortInfo") == 0 || strcmp(argv[i], "-npi") == 0) {
            if (i + 1 < argc) {
                nlPortInfo = argv[i + 1];
                ++i;
            } else {
                std::cerr << "Error: Missing argument for --nlPortInfo" << std::endl;
                return 1;
            }
        } else {
            std::cerr << "Error: Unknown argument: " << argv[i] << std::endl;
            display_help(programName);
            return 1;
        }
    }

    if (rtlPortInfo == nullptr || nlPortInfo == nullptr) {
        std::cerr << "Error: Missing required arguments." << std::endl;
        display_help(programName);
        return 1;
    }

    compare_ports(rtlPortInfo, nlPortInfo);
    return 0;
}