#include "dump_tcl.h"
int main(int argc, char* argv[]) {
  // Get the file name to work on
    std::string user_sdc   = "pc.pin";
    std::string output_tcl = "output.tcl";
    std::string output_json = "pin_map.json";

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-u" || arg == "--user_sdc") {
            if (i + 1 < argc) {
                user_sdc = argv[i + 1];
                i++;
            }
        } else if (arg == "-o" || arg == "--out") {
            if (i + 1 < argc) {
                output_tcl = argv[i + 1];
                i++;
            }
        } else if (arg == "-j" || arg == "--json") {
            if (i + 1 < argc) {
                output_json = argv[i + 1];
                i++;
            }
        }
    }
	dump_tcl (user_sdc, output_tcl, output_json);
}