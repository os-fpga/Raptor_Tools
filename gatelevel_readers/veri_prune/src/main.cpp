#include "veri_prune.h"
#include <fstream>

int main(int argc, char* argv[]) {
  // Get the file name to work on
const char* file_name = "../example/example.v";
    const char* out_file_name = "nl_to_vpr.v";
    const char* wrapper_file_name = "wrapper.v";
    std::string device_name = "gemini";
    std::string user_sdc = "example_1.txt";
    std::string pin_table = "pin_table.csv";
    std::string output_sdc = "output.sdc";

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-f" || arg == "--file") {
            if (i + 1 < argc) {
                file_name = argv[i + 1];
                i++;
            }
        } else if (arg == "-o" || arg == "--out") {
            if (i + 1 < argc) {
                out_file_name = argv[i + 1];
                i++;
            }
        } else if (arg == "-w" || arg == "--wrapper") {
            if (i + 1 < argc) {
                wrapper_file_name = argv[i + 1];
                i++;
            }
        } else if (arg == "-d" || arg == "--device") {
            if (i + 1 < argc) {
                device_name = argv[i + 1];
                i++;
            }
        } else if (arg == "-u" || arg == "--user_sdc") {
            if (i + 1 < argc) {
                user_sdc = argv[i + 1];
                i++;
            }
        } else if (arg == "-p" || arg == "--pin_table") {
            if (i + 1 < argc) {
                pin_table = argv[i + 1];
                i++;
            }
        } else if (arg == "-os" || arg == "--output_sdc") {
            if (i + 1 < argc) {
                output_sdc = argv[i + 1];
                i++;
            }
        }
    }

  std::string interface_data_dump_file(out_file_name);
  if (interface_data_dump_file.size() > 2 &&
      'v' == interface_data_dump_file.back() &&
      '.' == interface_data_dump_file[interface_data_dump_file.size() - 2]) {
    interface_data_dump_file.pop_back();
    interface_data_dump_file.pop_back();
  }
  interface_data_dump_file += "_interface.json";
  gb_constructs gb;
  prune_verilog(file_name, gb, device_name);
  write_sdc(user_sdc, pin_table, output_sdc ,gb );
  if (gb.contains_io_prem) {
    std::ofstream out_file;
    out_file.open(out_file_name);
    out_file << gb.mod_str;
    out_file.close();

    std::ofstream wrapper_file;
    wrapper_file.open(wrapper_file_name);
    wrapper_file << gb.intf_mod_str;
    wrapper_file << gb.top_mod_str;
    wrapper_file.close();

    std::ofstream interface_structure_file;
    interface_structure_file.open(interface_data_dump_file);
    interface_structure_file << gb.interface_data_dump;
    interface_structure_file.close();
  } else {
    std::cout << "Design does not contain any IO premitive" << std::endl;
  }
}