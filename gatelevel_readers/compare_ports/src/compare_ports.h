#ifndef _COMPARE_PORTS_H
#define _COMPARE_PORTS_H
#include <iostream>
#include "json.hpp"
#include <fstream>
#include <set>

void port_info(char* portInfo);
void hier_info(char* hierInfo);
std::set<std::map<std::string, std::string>> get_rtl_ports();
std::set<std::map<std::string, std::string>> get_nl_ports();
int compare_ports(char* portInfo, char* hierInfo);
#endif
