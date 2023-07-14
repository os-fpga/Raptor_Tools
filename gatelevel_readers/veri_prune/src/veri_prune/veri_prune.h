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
    std::vector<std::pair<std::string, std::map<std::string, int>>> gb_mods = {{"CLK_BUF", {{"I", 0}, {"O", 3}}}, 
        {"I_BUF",{{"I", 0}, {"C",4}, {"O", 1}}},
        {"I_BUF_DS", {{"OE",0}, {"I_N",0}, {"I_P",0}, {"O",1}}},
        {"I_DDR", {{"D",0}, {"R",5}, {"DLY_ADJ",0}, {"DLY_LD",0},{"DLY_INC",0}, {"C",4}, {"Q",1}}},
        {"I_SERDES", {{"D", 0}, {"RST", 0}, {"DPA_RST", 0}, {"FIFO_RST", 0}, {"DLY_LOAD", 0}, {"DLY_ADJ", 0}, {"DLY_INCDEC", 0},
          {"BITSLIP_ADJ", 0}, {"EN", 0}, {"CLK_IN", 0}, {"PLL_FAST_CLK", 0}, {"FAST_PHASE_CLK", 0}, {"PLL_LOCK", 0},
          {"CLK_OUT", 1}, {"CDR_CORE_CLK", 1}, {"Q", 1}, {"DATA_VALID", 1}, {"DLY_TAP_VALUE", 1}, {"DPA_LOCK", 1}, {"DPA_ERROR", 1}}},
        {"PLL", {{"PLL_EN", 0}, {"CLK_IN", 0}, {"CLK_OUT0_EN", 0}, {"CLK_OUT1_EN", 0}, {"CLK_OUT2_EN", 0}, {"CLK_OUT3_EN", 0},
          {"CLK_OUT0", 1}, {"CLK_OUT1", 1}, {"CLK_OUT2", 1}, {"CLK_OUT3", 1}, {"GEARBOX_FAST_CLK", 1}, {"LOCK", 1}}},
        {"IO_BUF", {{"I", 0}, {"T", 0}, {"IO", 2}, {"O", 1}}},
        {"IO_BUF_DS", {{"I", 0}, {"T", 0}, {"IOP", 2}, {"ION", 2}, {"O", 1}}},
        {"O_BUF", {{"I", 0}, {"C",4}, {"O", 1}}},
        {"O_BUFT", {{"I", 0}, {"OE", 0}, {"O", 1}}},
        {"O_BUFT_DS", {{"OE",0}, {"I",0}, {"C",4}, {"O_N",1}, {"O_P",1}}},
        {"O_DDR", {{"D",0}, {"R",5}, {"E",0}, {"DLY_ADJ",0},{"DLY_LD",0}, {"DLY_INC",0}, {"C",4}, {"Q",1}}},
        {"O_SERDES", {{"D", 0}, {"RST", 0}, {"LOAD_WORD", 0},{"DLY_LOAD", 0}, {"DLY_ADJ", 0}, {"DLY_INCDEC", 0},
          {"CLK_EN", 0}, {"CLK_IN", 0}, {"PLL_LOCK", 0}, {"PLL_FAST_CLK", 0}, {"FAST_PHASE_CLK", 0}, {"OE", 0},
          {"CLK_OUT", 1}, {"Q", 1}, {"DLY_TAP_VALUE", 1}, {"CHANNEL_BOND_SYNC_IN", 0}, {"CHANNEL_BOND_SYNC_OUT", 1}}}};
    std::vector<std::string> imods = {"CLK_BUF", "I_BUF", "I_BUF_DS", "I_DDR", "I_SERDES", "PLL"};
    std::vector<std::string> omods = {"O_BUF", "O_BUFT", "O_BUFT_DS", "O_DDR", "O_SERDES"};
    std::vector<std::string> iomods = {"IO_BUF", "IO_BUF_DS"};
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
    std::vector<std::string> gb_insts;
    std::vector<std::string> normal_insts;
    std::vector<std::pair<std::string, std::map<std::string, std::string>>> del_conns;
    char *mod_str;
    char *intf_mod_str;
    char *top_mod_str;
    bool contains_io_prem = false;
};
int prune_verilog (const char *file_name, gb_constructs &gb);
#endif