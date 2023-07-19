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

using Dir = Direction;

struct gb_constructs
{
    std::vector<std::pair<std::string, std::map<std::string, int>>> gb_mods = {{"CLK_BUF", {{"I", Dir::IN_DIR}, {"O", Dir::OUT_CLK}}}, 
        {"I_BUF",{{"I", Dir::IN_DIR}, {"C",Dir::IN_CLK}, {"O", Dir::OUT_DIR}}},
        {"I_BUF_DS", {{"OE",Dir::IN_DIR}, {"I_N",Dir::IN_DIR}, {"I_P",Dir::IN_DIR}, {"O",Dir::OUT_DIR}}},
        {"I_DDR", {{"D",Dir::IN_DIR}, {"R",Dir::IN_RESET}, {"DLY_ADJ",Dir::IN_DIR}, {"DLY_LD",Dir::IN_DIR},{"DLY_INC",Dir::IN_DIR}, {"C",Dir::IN_CLK}, {"Q",Dir::OUT_DIR}}},
        {"I_SERDES", {{"D", Dir::IN_DIR}, {"RST", Dir::IN_RESET}, {"DPA_RST", Dir::IN_DIR}, {"FIFO_RST", Dir::IN_RESET}, {"DLY_LOAD", Dir::IN_DIR}, {"DLY_ADJ", Dir::IN_DIR},
          {"DLY_INCDEC", Dir::IN_DIR}, {"BITSLIP_ADJ", Dir::IN_DIR}, {"EN", Dir::IN_DIR}, {"CLK_IN", Dir::IN_CLK}, {"PLL_FAST_CLK", Dir::IN_DIR},
          {"FAST_PHASE_CLK", Dir::IN_DIR}, {"PLL_LOCK", Dir::IN_DIR}, {"CLK_OUT", Dir::OUT_DIR}, {"CDR_CORE_CLK", Dir::OUT_DIR}, {"Q", Dir::OUT_DIR}, 
          {"DATA_VALID", Dir::OUT_DIR}, {"DLY_TAP_VALUE", Dir::OUT_DIR}, {"DPA_LOCK", Dir::OUT_DIR}, {"DPA_ERROR", Dir::OUT_DIR}}},
        {"PLL", {{"PLL_EN", Dir::IN_DIR}, {"CLK_IN", Dir::IN_CLK}, {"CLK_OUTDir::IN_DIR_EN", Dir::IN_DIR}, {"CLK_OUTOUT_DIR_EN", Dir::IN_DIR}, 
          {"CLK_OUTINOUT_DIR_EN", Dir::IN_DIR}, {"CLK_OUTOUT_CLK_EN", Dir::IN_DIR}, {"CLK_OUTDir::IN_DIR", Dir::OUT_DIR}, {"CLK_OUTOUT_DIR", Dir::OUT_DIR}, 
          {"CLK_OUTINOUT_DIR", Dir::OUT_DIR}, {"CLK_OUTOUT_CLK", Dir::OUT_DIR}, {"GEARBOX_FAST_CLK", Dir::OUT_DIR}, {"LOCK", Dir::OUT_DIR}}},
        {"IO_BUF", {{"I", Dir::IN_DIR}, {"T", Dir::IN_DIR}, {"IO", Dir::INOUT_DIR}, {"O", Dir::OUT_DIR}}},
        {"IO_BUF_DS", {{"I", Dir::IN_DIR}, {"T", Dir::IN_DIR}, {"IOP", Dir::INOUT_DIR}, {"ION", Dir::INOUT_DIR}, {"O", Dir::OUT_DIR}}},
        {"O_BUF", {{"I", Dir::IN_DIR}, {"C",Dir::IN_CLK}, {"O", Dir::OUT_DIR}}},
        {"O_BUFT", {{"I", Dir::IN_DIR}, {"OE", Dir::IN_DIR}, {"O", Dir::OUT_DIR}}},
        {"O_BUFT_DS", {{"OE",Dir::IN_DIR}, {"I",Dir::IN_DIR}, {"C",Dir::IN_CLK}, {"O_N",Dir::OUT_DIR}, {"O_P",Dir::OUT_DIR}}},
        {"O_DDR", {{"D",Dir::IN_DIR}, {"R",Dir::IN_RESET}, {"E",Dir::IN_DIR}, {"DLY_ADJ",Dir::IN_DIR},{"DLY_LD",Dir::IN_DIR}, {"DLY_INC",Dir::IN_DIR}, {"C",Dir::IN_CLK},
          {"Q",Dir::OUT_DIR}}},
        {"O_SERDES", {{"D", Dir::IN_DIR}, {"RST", Dir::IN_RESET}, {"LOAD_WORD", Dir::IN_DIR},{"DLY_LOAD", Dir::IN_DIR}, {"DLY_ADJ", Dir::IN_DIR}, {"DLY_INCDEC", Dir::IN_DIR},
          {"CLK_EN", Dir::IN_DIR}, {"CLK_IN", Dir::IN_CLK}, {"PLL_LOCK", Dir::IN_DIR}, {"PLL_FAST_CLK", Dir::IN_DIR}, {"FAST_PHASE_CLK", Dir::IN_DIR}, {"OE", Dir::IN_DIR},
          {"CLK_OUT", Dir::OUT_DIR}, {"Q", Dir::OUT_DIR}, {"DLY_TAP_VALUE", Dir::OUT_DIR}, {"CHANNEL_BOND_SYNC_IN", Dir::IN_DIR}, 
          {"CHANNEL_BOND_SYNC_OUT", Dir::OUT_DIR}}}};
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
    bool contains_io_prem = false;
};
int prune_verilog (const char *file_name, gb_constructs &gb);
#endif