#include "veri_prune.h"

int main(int argc, char **argv)
{
    // Get the file name to work on
    const char *file_name = (argc > 1) ? argv[1] : "../example/example.v" ;

    const char *out_file_name = (argc > 2) ? argv[2] : "to_vpr.v" ;
    const char *wrapper_file_name = (argc > 3) ? argv[3] : "wrapper.v" ;
    const char *file_base = ((argc > 3) ? argv[3] : "src") ;

    gb_constructs gb;
    prune_verilog (file_name, out_file_name, wrapper_file_name, file_base, gb);
}