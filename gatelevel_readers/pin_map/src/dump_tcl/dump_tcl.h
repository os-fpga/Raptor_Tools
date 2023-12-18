#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <map>

std::string get_ports(const std::string& port);
int dump_tcl(std::string& user_sdc, std::string& output_tcl);