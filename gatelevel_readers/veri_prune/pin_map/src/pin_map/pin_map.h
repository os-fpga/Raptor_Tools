#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>

struct orig_io
{
    std::string io_name;
    int         lsb;
    int         msb;
    unsigned dir;
};

int get_io_info(std::string mod_ios);
void print_ios();
