#include "compare_ports.h"
#include <iostream>
#include <string>

using namespace std;
int main(int argc, char** argv){

    char* portInfo = argv[1];
    char* hierInfo = argv[2];
    compare_ports(portInfo, hierInfo);
    return 0;
}



/*

.../design_data$ tclsh
% load ./libdesign.so
% test_block
test_block at line 9
% exit
.../design_data$ 

*/