
#include <cstring>
#include <iostream>
#include <regex>
#include <set>

#include "gb_map.h"
#include "kernel/celltypes.h"
#include "backends/rtlil/rtlil_backend.h"
#include "kernel/log.h"
#include "kernel/register.h"
#include "kernel/rtlil.h"
#include "kernel/yosys.h"
#include "kernel/mem.h"
#include "kernel/ffinit.h"
#include "kernel/ff.h"
#include "veri_prune.h"
#include <json.hpp>
#include <ostream>
using json = nlohmann::json;
USING_YOSYS_NAMESPACE
//PUBLIC_NAMESPACE_BEGIN

int prune_verilog(const char *file_name, gb_constructs &gb,
				const std::string &device_name) {
	std::cout << "Just started working on the tool" << std::endl;
	return 0; // Status OK
}
//PRIVATE_NAMESPACE_EN