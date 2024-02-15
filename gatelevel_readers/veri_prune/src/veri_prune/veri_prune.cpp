
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

struct EditingTool : public ScriptPass 
{
	EditingTool() : ScriptPass("veri_prune", "Netlist Editing Tool") {}

	void help() override
    {
        log("\n");
        log("This command runs power extraction for RapidSilicon FPGAs\n");
        log("\n");
        log("    -verilog <file>\n");
        log("        Write the design to the specified verilog file. writing of an output file\n");
        log("        is omitted if this parameter is not specified.\n");
        log("\n");
        log("\n");
    }

    string netlist_file;
	string interface_file;
   
	RTLIL::Design *_design;

	void clear_flags() override
    {
        netlist_file = "";
    }

	void execute(std::vector<std::string> args, RTLIL::Design *design) override
	{
		std::string run_from, run_to;
		clear_flags();
		_design = design;
		std::cout << "Execute Function override" << std::endl;

		size_t argidx;
        for (argidx = 1; argidx < args.size(); argidx++) {
			if ((args[argidx] == "-o" || args[argidx] == "--out") && argidx + 1 < args.size()) {
                interface_file = args[++argidx];
                continue;
            }
			if ((args[argidx] == "-w" || args[argidx] == "--wrapper") && argidx + 1 < args.size()) {
                interface_file = args[++argidx];
                continue;
            }
            break;
        }

		extra_args(args, argidx, design);


		run_script(design);
	}

	void script() override
    {
		std::cout << "Run Script" << std::endl;

		RTLIL::Module *module = _design->addModule("\\absval");
        log("Name of this module: %s\n", log_id(module));
		
		
	}
}EditingTool;

int prune_verilog(const char *file_name, gb_constructs &gb,
				const std::string &device_name) {
	std::cout << "Just started working on the tool" << std::endl;
	gb_constructs_data gb_mods_data;
	return 0; // Status OK
}