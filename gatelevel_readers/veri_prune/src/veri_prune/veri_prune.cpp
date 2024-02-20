
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
using namespace RTLIL;

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
	string wrapper_file;
	vector<Cell*> remove_prims;
   
	RTLIL::Design *_design;
	#ifdef GB_CONSTRUCTS_DATA
		gb_constructs_data gb_mods_data;
	#endif

	gb_mods_default gb;

	void clear_flags() override
    {
        netlist_file = "";
    }

	std::string remove_backslashes(const std::string& input) {
	    std::string result;
	    for (char c : input) {
	        if (c != '\\') {
	            result += c;
	        }
	    }
	    return result;
	}

	void delete_cells(Module* module, vector<Cell*> cells)
	{
		for (auto cell : cells)
		{
			module->remove(cell);
		}
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
                wrapper_file = args[++argidx];
                continue;
            }
            break;
        }
		extra_args(args, argidx, design);

		Module* original_mod = _design->top_module();
		for (auto cell :  original_mod->cells())
		{
			string module_name = remove_backslashes(cell->type.str());
			if(std::find(gb.primitives.begin(), gb.primitives.end(), module_name) != gb.primitives.end())
			{
				remove_prims.push_back(cell);
			}
		}

		delete_cells(original_mod, remove_prims);

		run_script(design);
	}

	void script() override
    {
		std::cout << "Run Script" << std::endl;	
	}
}EditingTool;