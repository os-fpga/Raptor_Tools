#include "veri_prune.h"
#include <fstream>

int main(int argc, char **argv) {
  // Get the file name to work on
  const char *file_name = (argc > 1) ? argv[1] : "../example/example.v";

  const char *out_file_name = (argc > 2) ? argv[2] : "nl_to_vpr.v";
  const char *wrapper_file_name = (argc > 3) ? argv[3] : "wrapper.v";
  std::string device_name = ((argc > 7) ? argv[7] : "gemini");
  std::string user_sdc= ((argc > 4) ? argv[4] : "example_1.txt");
  std::string pin_table=((argc > 5) ? argv[5] : "pin_table.csv");
  std::string output_sdc=((argc > 6) ? argv[6] : "output.sdc");

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