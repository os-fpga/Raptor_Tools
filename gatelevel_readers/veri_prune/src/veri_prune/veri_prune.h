#ifndef VERI_PRUNE_H
#define VERI_PRUNE_H

#include <iostream>
#include <sstream> // std::stringstream, std::stringbuf
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <algorithm>

using namespace std;
struct orig_io
{
    std::string io_name;
    int         lsb;
    int         msb;
    std::string dir;
};

struct gb_constructs
{
    std::vector<std::pair<std::string, int>> mod_ios;
    std::vector<std::string> mod_clks;
    std::vector<std::string> clk_in;
    std::vector<std::string> intf_ins;
    std::vector<std::string> intf_outs;
    std::vector<std::pair<std::string, std::vector<int>>> indexed_mod_ios;
    std::vector<std::pair<std::string, std::vector<int>>> indexed_mod_clks;
    std::vector<std::pair<std::string, std::vector<int>>> indexed_intf_ins;
    std::vector<std::pair<std::string, std::vector<int>>> indexed_intf_outs;
    std::vector<std::string> intf_inouts;
    std::vector<std::string> mod_ports;
    std::vector<std::string> top_ports;
    std::vector<std::string> prefs;
    std::unordered_set<std::string> assign_nets;
    std::unordered_set<std::string> inst_nets;
    std::unordered_set<std::string> del_ports;
    std::unordered_set<std::string> intf_ports;
    std::vector<std::string> gb_insts;
    std::vector<std::string> normal_insts;
    std::unordered_set<std::string> intf_ios;
    std::unordered_set<std::string> top_ios;
    std::vector<std::string> remove_intf_nets;
    std::vector<std::string> remove_top_nets;
    char *mod_str;
    char *intf_mod_str;
    char *top_mod_str;
    std::string interface_data_dump;
    bool contains_io_prem = false;
};
int prune_verilog (const char *file_name, gb_constructs &gb, const std::string& device_name);
#endif
