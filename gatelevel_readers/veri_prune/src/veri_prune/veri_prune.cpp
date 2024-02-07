
#include <cstring>
#include <iostream>
#include <regex>
#include <set>

#include "gb_map.h"
#include "veri_prune.h"
#include <json.hpp>
#include <ostream>
using json = nlohmann::json;


int prune_verilog(const char *file_name, gb_constructs &gb,
				const std::string &device_name) {
	std::cout << "Just started working on the tool" << std::endl;
	return 0; // Status OK
}