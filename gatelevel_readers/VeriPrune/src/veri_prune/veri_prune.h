#ifndef ADD_AND_DELETE_STATEMENTS
#define ADD_AND_DELETE_STATEMENTS

#include <iostream>
#include <sstream> // std::stringstream, std::stringbuf
#include <string>
#include <vector>

using namespace std;

struct gb_constructs
{
    std::vector <std::string> gb_mods = {"clkbuf", "ibuf", "ibufds",
                        "iddr", "obuf", "obuftds", "obuft", "oddr"};
};
int prune_verilog (const char *file_name, const char *out_file_name, const char *file_base, gb_constructs &gb);

#endif