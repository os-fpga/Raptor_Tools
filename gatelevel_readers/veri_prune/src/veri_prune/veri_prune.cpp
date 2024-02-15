
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
        log("This command runs Netlist editing tool\n");
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
		// TODO: Will send the arguments and test after parsing is done 
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

		for (auto mod : _design->modules())
		{
			// Iterate over all cells in the module
			
    		for (auto cell :  mod->cells()) {
    		    if (cell->type == RTLIL::escape_id("I_BUF")) {
					// Print the instance and module name
					log("Instance name: %s  of Module: %s\n ", log_id(cell->name), log_id(cell->type));
				}
    		}
		}

		
		
		
	}
}EditingTool;