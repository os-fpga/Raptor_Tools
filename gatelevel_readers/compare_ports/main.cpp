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