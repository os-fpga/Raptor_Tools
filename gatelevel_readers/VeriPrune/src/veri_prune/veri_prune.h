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

struct gb_constructs
{
    std::vector<std::pair<std::string, std::map<std::string, int>>> gb_mods = {{"clkbuf", {{"I", 0}, {"O", 1}}}, 
        {"ibuf",{{"I", 0}, {"C", 0}, {"O", 1}}},
        {"ibufds", {{"OE",0}, {"I_N",0}, {"I_P",0}, {"O",1}}},
        {"iddr", {{"D",0}, {"R",0}, {"DLY_ADJ",0}, {"DLY_LD",0},{"DLY_INC",0}, {"C",0}, {"Q0",1}, {"Q1",1}}},
        {"obuf", {{"I", 0}, {"C", 0}, {"O", 1}}},
        {"obuftds", {{"OE",0}, {"I",0}, {"C",0}, {"O_N",1}, {"O_P",1}}},
        {"obuft", {{"I", 0}, {"OE", 0}, {"O", 1}}},
        {"oddr", {{"D0",0}, {"D1",0}, {"R",0}, {"E",0}, {"DLY_ADJ",0},{"DLY_LD",0}, {"DLY_INC",0}, {"C",0}, {"Q",1}}}
        };
    std::vector<std::string> imods = {"clkbuf", "ibuf", "ibufds", "iddr"};
    std::vector<std::string> omods = {"obuf", "obuftds", "obuft", "oddr"};
    std::unordered_set<std::string> insts_visited;
    std::vector<std::string> prefs;
    std::vector<std::string> top_ports;
    std::vector<std::string> wrapper_conns;
    std::unordered_set<std::string> del_ports;
    std::vector<std::pair<std::string, std::map<std::string, std::string>>> del_conns;
};
int prune_verilog (const char *file_name, const char *out_file_name, const char *file_base, gb_constructs &gb);

#endif