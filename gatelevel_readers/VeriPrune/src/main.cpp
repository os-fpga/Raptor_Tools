#include "add_and_delete_statements.h"

int main(int argc, char **argv)
{
    // Get the file name to work on
    const char *file_name = (argc > 1) ? argv[1] : "../example/example.v" ;

    const char *out_file_name = (argc > 2) ? argv[2] : "ex2_out.v" ;
    const char *file_base = ((argc > 3) ? argv[3] : "src") ;
    prune_verilog (file_name, out_file_name, file_base);
}