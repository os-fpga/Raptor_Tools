#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "simple_netlist.h"

std::map<std::string, const char*> parseArguments(int argc, char **argv)
{
    std::map<std::string, const char*> arguments = {{"vfile", nullptr},{"key", "./private_key.pem"},{"out", nullptr},{"top", nullptr} };
    
    for (int i = 1; i < argc; i += 2)
    {
        std::string arg = argv[i];
        
        if (arg.substr(0, 2) == "--")
        {
            if (i + 1 < argc)
            {
                arguments[arg.substr(2)] = argv[i + 1];
            }
        }
    }
    
    return arguments;
}

int main(int argc, char **argv)
{
    std::map<std::string, const char*> arguments = parseArguments(argc, argv);
    const char *file_name   = arguments["vfile"];
    const char *key_file    = arguments["key"];
    const char *out_file    = arguments["out"];
    const char *top_mod     = arguments["top"];

    if(file_name == nullptr) {
        std::cout << "Usage : read_verilog \t--vfile <file_name> [options]" << std::endl;
        std::cout << "Options : \t\t--key <encryption private key file> \t// defaults to ./private_key.pem" << std::endl;
        std::cout << "\t\t\t--out <output file> \t\t\t// defaults to nullptr/std::out" << std::endl;
        std::cout << "\t\t\t--top <top module> \t\t\t// defaults to nullptr" << std::endl;
        return -1;
    }

    simple_netlist n_l;
    parse_verilog(file_name, n_l, key_file, top_mod);
    if (out_file != nullptr)
    {
        ofstream outfile(out_file, ios::out);
        if (outfile.is_open())
        {
            n_l.b_print(outfile);
            outfile.close();
        }
    }
    else
        n_l.b_print(std::cout);
    return 0;
}