#include <iostream>
#include <fstream>
#include "simple_netlist.h"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        return 4;
    }
    const char *file_name = argv[1]; // Set the file name as specified by the user
    simple_netlist n_l;
    parse_verilog(file_name, n_l);
    if (argc > 2)
    {
        ofstream outfile(argv[2], ios::out);
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
