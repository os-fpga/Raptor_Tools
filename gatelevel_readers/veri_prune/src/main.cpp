#include "veri_prune.h"
#include <fstream>

int main(int argc, char **argv)
{
    // Get the file name to work on
    const char *file_name = (argc > 1) ? argv[1] : "../example/example.v" ;

    const char *out_file_name = (argc > 2) ? argv[2] : "nl_to_vpr.v" ;
    const char *wrapper_file_name = (argc > 3) ? argv[3] : "wrapper.v" ;
    const char *file_base = ((argc > 3) ? argv[3] : "src") ;

    gb_constructs gb;
    prune_verilog (file_name, gb);

    if (gb.contains_io_prem) {
        std::ofstream out_file ;
        out_file.open(out_file_name) ;
        out_file << gb.mod_str;
        
        std::ofstream wrapper_file;
        wrapper_file.open(wrapper_file_name);
        wrapper_file << gb.intf_mod_str;
        wrapper_file << gb.top_mod_str;
    } else {
        std::cout << "Design does not contain any IO premitive" << std::endl;
    }
}