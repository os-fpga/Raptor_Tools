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

struct primitives_data
{
    std::map<std::string, std::set<std::string>> io_primitives = {{"genesis3", 
        {"CLK_BUF", "I_BUF", "I_BUF_DS", "I_DDR", "I_DELAY", "I_SERDES", "O_BUF", "O_BUFT", "O_BUFT_DS",
         "O_BUF_DS", "O_DDR", "O_DELAY", "O_SERDES", "O_SERDES_CLK", "PLL"}}};

    // Function to get the primitive names for a specific cell library
    std::set<std::string> get_primitives(const std::string& lib) {
        std::set<std::string> primitive_names;
        auto it = io_primitives.find(lib);
        if (it != io_primitives.end()) {
            primitive_names = it->second;
        }
        return primitive_names;
    }
};

#endif
