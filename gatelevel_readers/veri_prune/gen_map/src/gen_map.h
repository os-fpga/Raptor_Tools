#include <cstring>
#include <filesystem>
#include <dirent.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <unordered_map>

#define IN_DIR  0
#define OUT_DIR  1
#define INOUT_DIR  2
#define OUT_CLK  3
#define IN_CLK  4
#define IN_RESET  5

using namespace std;

struct gb_map
{
    std::vector<std::pair<std::string, std::map<std::string, int>>> gb_mods;
    std::map<std::string, std::vector<std::pair<std::string, std::map<std::string, int>>>> device_premitives;
    std::unique_ptr<char[]> contentArray;

    ~gb_map()
    {
        contentArray.reset();
    }
};