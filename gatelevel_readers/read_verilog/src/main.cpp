#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "simple_netlist.h"

std::map<std::string, std::string> parseArguments(int argc, char **argv)
{
    std::map<std::string, std::string> arguments;
    
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
    std::map<std::string, std::string> arguments = parseArguments(argc, argv);
    const char *file_name  = arguments.count("vfile") > 0 ? arguments["vfile"].c_str() : nullptr;
    const char *key_file = arguments.count("key") > 0 ? arguments["key"].c_str() : "./private_key.pem";
    const char *out_file = arguments.count("out") >0 ? arguments["out"].c_str() : nullptr;
    const char *top_mod = arguments.count("top") >0 ? arguments["top"].c_str() : nullptr;

    if(file_name == nullptr) {
        throw(std::invalid_argument("Give input file name as --vfile"));
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
    //else
    //    n_l.b_print(std::cout);
    return 0;
}
