#include <string>
#include <fstream>
#include <iostream>
struct orig_io
{
    std::string io_name;
    int         lsb;
    int         msb;
    std::string dir;
};

int get_io_info(std::string mod_ios);
void print_ios();
