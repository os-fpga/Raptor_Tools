#ifndef DEVICE_PRIMITIVES_DATA_H
#define DEVICE_PRIMITIVES_DATA_H
#include <vector>
#include <map>
#define IN_DIR  0
#define OUT_DIR  1
#define INOUT_DIR  2
#define OUT_CLK  3
#define IN_CLK  4
#define IN_RESET  5

struct gb_mods_default
{
    std::vector<std::pair<std::string, std::map<std::string, int>>> gb_mods = {{"CLK_BUF", {{"I", IN_DIR}, {"O", OUT_CLK}}}, 
        {"I_BUF",{{"I", IN_DIR}, {"C",IN_CLK}, {"O", OUT_DIR}}},
        {"I_BUF_DS", {{"OE",IN_DIR}, {"I_N",IN_DIR}, {"I_P",IN_DIR}, {"O",OUT_DIR}}},
        {"I_DDR", {{"D",IN_DIR}, {"R",IN_RESET}, {"DLY_ADJ",IN_DIR}, {"DLY_LD",IN_DIR},{"DLY_INC",IN_DIR}, {"C",IN_CLK}, {"Q",OUT_DIR}}},
        {"I_SERDES", {{"D", IN_DIR}, {"RST", IN_RESET}, {"DPA_RST", IN_DIR}, {"FIFO_RST", IN_RESET}, {"DLY_LOAD", IN_DIR}, {"DLY_ADJ", IN_DIR},
          {"DLY_INCDEC", IN_DIR}, {"BITSLIP_ADJ", IN_DIR}, {"EN", IN_DIR}, {"CLK_IN", IN_CLK}, {"PLL_FAST_CLK", IN_DIR},
          {"FAST_PHASE_CLK", IN_DIR}, {"PLL_LOCK", IN_DIR}, {"CLK_OUT", OUT_DIR}, {"CDR_CORE_CLK", OUT_DIR}, {"Q", OUT_DIR}, 
          {"DATA_VALID", OUT_DIR}, {"DLY_TAP_VALUE", OUT_DIR}, {"DPA_LOCK", OUT_DIR}, {"DPA_ERROR", OUT_DIR}}},
        {"PLL", {{"PLL_EN", IN_DIR}, {"CLK_IN", IN_CLK}, {"CLK_OUT0_EN", IN_DIR}, {"CLK_OUT1_EN", IN_DIR}, 
          {"CLK_OUT2_EN", IN_DIR}, {"CLK_OUT3_EN", IN_DIR}, {"CLK_OUT0", OUT_DIR}, {"CLK_OUT1", OUT_DIR}, 
          {"CLK_OUT2", OUT_DIR}, {"CLK_OUT3", OUT_DIR}, {"GEARBOX_FAST_CLK", OUT_DIR}, {"LOCK", OUT_DIR}}},
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
};

#endif // DEVICE_PRIMITIVES_DATA_H
