#include "pin_map.h"
#include <json.hpp>
using json = nlohmann::json;

std::vector<orig_io> orig_ios;

int get_io_info(std::string mod_ios)
{
    std::ifstream jsonFile(mod_ios);

    // Check if the file was opened successfully.
    if (!jsonFile.is_open()) {
        std::cerr << "Failed to open the JSON file." << std::endl;
        return 1;
    }

    // Parse the JSON data from the file.
    json parsedJson;
    jsonFile >> parsedJson;

    // Close the file after parsing.
    jsonFile.close();

    json portsArray = parsedJson[0]["ports"];

    for (const auto& port : portsArray) {
        orig_io io_info;
        io_info.dir = port["direction"];
        io_info.io_name = port["name"];
        io_info.lsb = port["range"]["lsb"];
        io_info.msb = port["range"]["msb"];
        orig_ios.push_back(io_info);
    }

    return 0;
}

void print_ios()
{
    for (const orig_io& entry : orig_ios)
    {
        std::string io_name = entry.io_name;
        unsigned lsb = entry.lsb;
        unsigned msb = entry.msb;
        std::string dir = entry.dir;
        std::cout << "io_name: " << io_name << " lsb: " << lsb << " msb: " << msb << " dir: " << dir << std::endl;
    }
}