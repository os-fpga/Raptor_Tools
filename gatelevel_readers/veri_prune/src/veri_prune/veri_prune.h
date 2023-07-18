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

enum Direction : unsigned int {
    IN_DIR = 0,
    OUT_DIR = 1,
    INOUT_DIR = 2,
    OUT_CLK = 3,
    IN_CLK = 4,
    IN_RESET = 5
};

struct gb_constructs
{
    std::vector<std::pair<std::string, std::map<std::string, int>>> gb_mods = {{"CLK_BUF", {{"I", IN_DIR}, {"O", OUT_CLK}}}, 
        {"I_BUF",{{"I", IN_DIR}, {"C",IN_CLK}, {"O", OUT_DIR}}},
        {"I_BUF_DS", {{"OE",IN_DIR}, {"I_N",IN_DIR}, {"I_P",IN_DIR}, {"O",OUT_DIR}}},
        {"I_DDR", {{"D",IN_DIR}, {"R",IN_RESET}, {"DLY_ADJ",IN_DIR}, {"DLY_LD",IN_DIR},{"DLY_INC",IN_DIR}, {"C",IN_CLK}, {"Q",OUT_DIR}}},
        {"I_SERDES", {{"D", IN_DIR}, {"RST", IN_RESET}, {"DPA_RST", IN_DIR}, {"FIFO_RST", IN_RESET}, {"DLY_LOAD", IN_DIR}, {"DLY_ADJ", IN_DIR},
          {"DLY_INCDEC", IN_DIR}, {"BITSLIP_ADJ", IN_DIR}, {"EN", IN_DIR}, {"CLK_IN", IN_CLK}, {"PLL_FAST_CLK", IN_DIR},
          {"FAST_PHASE_CLK", IN_DIR}, {"PLL_LOCK", IN_DIR}, {"CLK_OUT", OUT_DIR}, {"CDR_CORE_CLK", OUT_DIR}, {"Q", OUT_DIR}, 
          {"DATA_VALID", OUT_DIR}, {"DLY_TAP_VALUE", OUT_DIR}, {"DPA_LOCK", OUT_DIR}, {"DPA_ERROR", OUT_DIR}}},
        {"PLL", {{"PLL_EN", IN_DIR}, {"CLK_IN", IN_CLK}, {"CLK_OUTIN_DIR_EN", IN_DIR}, {"CLK_OUTOUT_DIR_EN", IN_DIR}, 
          {"CLK_OUTINOUT_DIR_EN", IN_DIR}, {"CLK_OUTOUT_CLK_EN", IN_DIR}, {"CLK_OUTIN_DIR", OUT_DIR}, {"CLK_OUTOUT_DIR", OUT_DIR}, 
          {"CLK_OUTINOUT_DIR", OUT_DIR}, {"CLK_OUTOUT_CLK", OUT_DIR}, {"GEARBOX_FAST_CLK", OUT_DIR}, {"LOCK", OUT_DIR}}},
        {"IO_BUF", {{"I", IN_DIR}, {"T", IN_DIR}, {"IO", INOUT_DIR}, {"O", OUT_DIR}}},
        {"IO_BUF_DS", {{"I", IN_DIR}, {"T", IN_DIR}, {"IOP", INOUT_DIR}, {"ION", INOUT_DIR}, {"O", OUT_DIR}}},
        {"O_BUF", {{"I", IN_DIR}, {"C",IN_CLK}, {"O", OUT_DIR}}},
        {"O_BUFT", {{"I", IN_DIR}, {"OE", IN_DIR}, {"O", OUT_DIR}}},
        {"O_BUFT_DS", {{"OE",IN_DIR}, {"I",IN_DIR}, {"C",IN_CLK}, {"O_N",OUT_DIR}, {"O_P",OUT_DIR}}},
        {"O_DDR", {{"D",IN_DIR}, {"R",IN_RESET}, {"E",IN_DIR}, {"DLY_ADJ",IN_DIR},{"DLY_LD",IN_DIR}, {"DLY_INC",IN_DIR}, {"C",IN_CLK},
          {"Q",OUT_DIR}}},
        {"O_SERDES", {{"D", IN_DIR}, {"RST", IN_RESET}, {"LOAD_WORD", IN_DIR},{"DLY_LOAD", IN_DIR}, {"DLY_ADJ", IN_DIR}, {"DLY_INCDEC", IN_DIR},
          {"CLK_EN", IN_DIR}, {"CLK_IN", IN_CLK}, {"PLL_LOCK", IN_DIR}, {"PLL_FAST_CLK", IN_DIR}, {"FAST_PHASE_CLK", IN_DIR}, {"OE", IN_DIR},
          {"CLK_OUT", OUT_DIR}, {"Q", OUT_DIR}, {"DLY_TAP_VALUE", OUT_DIR}, {"CHANNEL_BOND_SYNC_IN", IN_DIR}, 
          {"CHANNEL_BOND_SYNC_OUT", OUT_DIR}}}};
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
    char *mod_str;
    char *intf_mod_str;
    char *top_mod_str;
    bool contains_io_prem = false;
};
int prune_verilog (const char *file_name, gb_constructs &gb);
#endif