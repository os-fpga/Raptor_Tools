#include "pin_map.h"
int main(int argc, char* argv[]) {
  // Get the file name to work on
    std::string intf_json  = "interface.json";
    std::string mod_ios    = "mod_ios.json";
    std::string map_json   = "pin_map.json";
    std::string pin_table  = "pin_table.csv";
    std::string output_sdc = "output.sdc";

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-i" || arg == "--intf") {
            if (i + 1 < argc) {
                intf_json = argv[i + 1];
                i++;
            }
        } else if (arg == "-io" || arg == "--ios") {
            if (i + 1 < argc) {
                mod_ios = argv[i + 1];
                i++;
            }
        } else if (arg == "-j" || arg == "--map_json") {
            if (i + 1 < argc) {
                map_json = argv[i + 1];
                i++;
            }
        } else if (arg == "-t" || arg == "--ptable") {
            if (i + 1 < argc) {
                pin_table = argv[i + 1];
                i++;
            }
        } else if (arg == "-o" || arg == "--out") {
            if (i + 1 < argc) {
                output_sdc = argv[i + 1];
                i++;
            }
        }
    }
	update_sdc (intf_json, mod_ios, map_json, pin_table, output_sdc);
}