
#include <cstring>
#include <iostream>
#include <regex>
#include <set>

#include "gb_map.h"
#include "veri_prune.h"
#include <json.hpp>
#include <ostream>
using json = nlohmann::json;

//#define dir_inout 329
//#define dir_input 330
//#define dir_output 346
//#define is_wire 392
//
//std::vector<orig_io> orig_ios;
//std::map<std::string, std::vector<Connection>> instConns;
//
//std::unordered_map<int, std::string> directions = {
//  {dir_input, "Input"},
//  {dir_output, "Output"},
//  {dir_inout, "Inout"}
//};
//std::map<std::string, std::string> myMap; // Define myMap at a global scope
//std::string get_ports; // Define get_ports at a global scope
//struct ioInfo {
//    std::string ioName;
//    std::string actualName;
//    std::string ioDir;
//    unsigned lsb;
//    unsigned msb;
//};
//std::map<std::string, std::vector<ioInfo>> instIOs;

//void getInstIos (const json& jsonData)
//{
//  for (const auto& [instanceName, instanceInfo] : jsonData["IO_Instances"].items())
//    {
//        const json& ports = instanceInfo["ports"];
//        std::string modName = instanceInfo["module"];
//        for (const auto& [portName, portInfo] : ports.items()) {
//            std::string actualSignal = portInfo["Actual"];
//            if (portInfo.contains("FUNC")) {
//                std::string sigDir = portInfo["FUNC"];
//                unsigned lsb = portInfo["lsb"];
//                unsigned msb = portInfo["msb"];
//                ioInfo pInfo;
//                pInfo.actualName = actualSignal;
//                pInfo.ioName = portName;
//                pInfo.ioDir = sigDir;
//                pInfo.lsb = lsb;
//                pInfo.msb = msb;
//                instIOs[instanceName].push_back(pInfo);
//            }
//        }
//    }
//}

//void printInstIos()
//{
//    for (const auto& entry : instIOs) {
//        std::cout << "Instance: " << entry.first << std::endl;
//        for (const auto& io : entry.second) {
//            std::cout << "  Port Name: " << io.ioName << std::endl;
//            std::cout << "  Actual Name: " << io.actualName << std::endl;
//            std::cout << "  IO Direction: " << io.ioDir << std::endl;
//            std::cout << "  LSB :  " << io.lsb << std::endl;
//            std::cout << "  MSB :  " << io.msb << std::endl;
//        }
//    }
//}

//std::vector<std::string> direction_print_outs = {
//    "IN_DIR", "OUT_DIR", "INOUT_DIR", "OUT_CLK", "IN_CLK", "IN_RESET"};
//std::unordered_map<std::string, std::map<std::string, int>>
//    current_primitive_spec_map;

//void keep_nets(mod *mod, std::unordered_set<std::string> io_intf,
//               gb_constructs &gb, std::vector<std::string> &remove_nets) {
//  std::unordered_set<std::string> keep_intf_ios;
//  keep_intf_ios.insert(gb.intf_ios.begin(), gb.intf_ios.end());
//  keep_intf_ios.insert(io_intf.begin(), io_intf.end());
//  
//  // Remove signals not found in keep_intf_nets
//}

//void remove_mod_clks(
//    std::vector<std::pair<std::string, std::vector<int>>> &indexed_data,
//    std::vector<std::string> &mod_clks) {
//  indexed_data.erase(
//      std::remove_if(
//          indexed_data.begin(), indexed_data.end(),
//          [&mod_clks](const std::pair<std::string, std::vector<int>> &pair) {
//            return std::find(mod_clks.begin(), mod_clks.end(), pair.first) !=
//                   mod_clks.end();
//          }),
//      indexed_data.end());
//}

//void add_idx_intf_ports(
//    mod *intf_mod,
//    std::vector<std::pair<std::string, std::vector<int>>> &indexed_intf_ios,
//    unsigned dir, std::unordered_set<std::string> &ports_intf) {
//  for (const auto &pair : indexed_intf_ios) {
//    const auto &values = pair.second;
//    unsigned msb;
//    unsigned lsb;
//    msb = values[0];
//    lsb = values[1];
//    // Add port
//    ports_intf.insert(pair.first);
//  }
//}

//void add_intf_ports() {
//  for (const auto &port : intf_ios) {
//    // Add port
//    ports_intf.insert(port);
//  }
//}

//void add_mod_ports() {
//  for (const auto &pair : indexed_mod_ios) {
//    const auto &values = pair.second;
//    unsigned msb;
//    unsigned lsb;
//    unsigned dir;
//    if (values.size() == 3) {
//      msb = values[0];
//      lsb = values[1];
//      dir = values[2];
//    } else {
//      msb = values[0];
//      lsb = values[1];
//      dir = dir_input;
//    }
//    // Add port
//  }
//}

//void gather_data() {
//  // Fillout data
//}

////////////////////////////////////////////////////////////////////////////

//std::pair<std::string, std::string> parseKeyValue(const std::string &input) {
//  size_t keyStart = input.find_first_not_of('.');
//  size_t keyEnd = input.find('(');
//
//  if (keyStart == std::string::npos || keyEnd == std::string::npos) {
//    // Invalid format
//    return {"", ""};
//  }
//
//  std::string key = input.substr(keyStart, keyEnd - keyStart);
//
//  size_t valStart = keyEnd + 1;
//  size_t valEnd = input.find(')', valStart);
//
//  if (valStart == std::string::npos || valEnd == std::string::npos) {
//    // Invalid format
//    return {"", ""};
//  }
//
//  std::string value = input.substr(valStart, valEnd - valStart);
//
//  return {key, value};
//}

//void print_out_io_primitives( *intf_mod, gb_constructs &gb) {
//  std:stringstream out_stream;
//  out_stream << "{\n    \"IO_Instances\": {" << std::endl;
//  unsigned idx;
//  bool firstInst = true;
//
//  foreach(prim_items, idx, prim_item) {
//    if (!prim_item)
//      continue;
//    if (prim_item(isInstance)) {
//
//      // out_stream << __LINE__ << " &&&& ";
//      // prim_item->PrettyPrint(out_stream, 0);
//
//      // Get instances
//      foreach(inst in insts) {
//
//        std::string mod_name
//        std::string no_param_name = "";
//
//        // reducing a correctly named parametrized module MyModule(par1=99) to
//        // MyModule discuss with thierry !
//        for (auto k : mod_name)
//          if ('(' == k)
//            break;
//          else
//            no_param_name.push_back(k);
//
//        std::map<std::string, int> m_items;
//        const char *inst_name
//        if (!firstInst) {
//          out_stream << ",\n";
//        }
//        out_stream << "        ";
//        firstInst = false;
//        out_stream << "\"" << inst_name << "\": {" << std::endl;
//        out_stream << "            \"module\": \"" << no_param_name << "\",";
//
//        // Fillout params
//        
//        // Fillout ports information
//        out_stream << "        }";
//      }
//    }
//  }
//  out_stream << "\n    }\n}" << std::endl;
//  gb.interface_data_dump = out_stream.str();
//}

//void printPinMap ()
//{
//	for (const auto& entry : instConns) {
//        const std::string& instance = entry.first;
//        const std::vector<Connection>& instanceConnections = entry.second;
//
//        std::cout << "Instance: " << instance << std::endl;
//        for (const auto& conn : instanceConnections) {
//            std::cout << "Signal: " << conn.signal << std::endl;
//            std::cout << "Module: " << conn.module << std::endl;
//            // Print other fields as needed
//            for (const auto& port : conn.ports) {
//                std::cout << "  " << port.first << ": " << port.second << std::endl;
//        }
//        }
//    }
//}

////////////////////////////////////////////////////////////////////////////

int prune_verilog(const char *file_name, gb_constructs &gb,
                  const std::string &device_name) {
// Removing verific dependent code and commenting simple code and comments
//gb_mods_default default_mods;
#ifdef GB_CONSTRUCTS_DATA
  gb_constructs_data gb_mods_data;
#endif

  // Get all the top level modules
  // Get a handle of the first top level module, if any.

  //json j_module;
  // save original IOs in orig_ios

  // Now copy the top level module to create interface module

  //////////////////////////// Remove assign statements from interface and
  /// wrapper modules /////////////////////
  // Get the module item list of module.

  // Get the module item list of module.
  

  //////////////////////////////////////// Get the nets' names used in assign
  /// statements /////////////////////
    
  /////////////////////////
  //if (!gb.contains_io_prem)
  //  return 0;
  /////////////////////////

  ////////////////////////////// Remove gearbox modules' instances from original
  /// module ////////////////////////////

  ///////////////////////////// Get the nets connected to the remaining
  /// instances //////////////////////////////////

  //////////// Check if an output of the interface module is also an input of
  /// the same module ////////////
  //std::unordered_set<std::string>
  //    io_intf; // To store duplicate IOs of interface module
  //for (const std::string &element : gb.intf_outs) {
  //  if (std::find(gb.intf_ins.begin(), gb.intf_ins.end(), element) !=
  //      gb.intf_ins.end()) {
  //    io_intf.insert(element);
  //  }
  //}

  // Iterate over indexed_intf_outs
  //for (const auto &out_pair : gb.indexed_intf_outs) {
  //  const std::string &out_first = out_pair.first;
  //  // Iterate over indexed_intf_ins
  //  for (const auto &in_pair : gb.indexed_intf_ins) {
  //    const std::string &in_first = in_pair.first;
  //    // Compare the first elements of the pairs
  //    if (out_first == in_first) {
  //      // Push the first element to the set
  //      io_intf.insert(out_first);
  //      break; // No need to continue searching for the same out_first
  //    }
  //  }
  //}

  /////////////// Remove it from the port list of the original module
  /////////////////////////////////////
  // Iterate over the vector and check if the first element exists in the set
  //for (auto it = gb.indexed_mod_ios.begin(); it != gb.indexed_mod_ios.end();) {
  //  const std::string &firstElement = it->first;
  //  if (io_intf.count(firstElement) > 0) {
  //    it = gb.indexed_mod_ios.erase(it); // Remove the pair from the vector
  //  } else {
  //    ++it;
  //  }
  //}

  //auto remove_if_found = [&io_intf](const std::pair<std::string, int> &pair) {
  //  return io_intf.find(pair.first) != io_intf.end();
  //};

  //gb.mod_ios.erase(
  //    std::remove_if(gb.mod_ios.begin(), gb.mod_ios.end(), remove_if_found),
  //    gb.mod_ios.end());
  /////////////////////////////////////////////// BLOCK END
  //////////////////////////////////////////////////

  //////////////////////// Remove the output of the clock buffer from module IOs
  ///////////////////////////
  //std::vector<std::string> mod_clks;
  //for (const auto &pair : gb.indexed_mod_clks) {
  //  mod_clks.push_back(pair.first);
  //}

  // Remove elements from indexed_mod_ios if their first element matches any in
  // mod_clks
  //remove_mod_clks(gb.indexed_mod_ios, mod_clks);

  // Iterate over mod_clks
  //for (const auto &clk : gb.mod_clks) {
  //  // Find the pair with matching first element in mod_ios
  //  auto it = std::find_if(gb.mod_ios.begin(), gb.mod_ios.end(),
  //                         [&clk](const std::pair<std::string, int> &p) {
  //                           return p.first == clk;
  //                         });
  //  // If a match is found, remove the pair from mod_ios
  //  if (it != gb.mod_ios.end()) {
  //    gb.mod_ios.erase(it);
  //  }
  //}
  ////////////////////////////////// BLOCK END
  ////////////////////////////////////////////////////////////////

  ////////////////////////// Add ports to the original module
  ////////////////////////////////////////////////
  //add_mod_ports(mod, gb.indexed_mod_ios);
  //add_mod_ports(mod, gb.indexed_mod_clks);

  //for (const auto &pair : gb.mod_ios) {
  //  //Add Port with proper dir
  //}

  //for (const auto &port : gb.mod_clks) {
  //  //Add port as input
  //}
  ////////////////////////////////////// BLOCK END
  ////////////////////////////////////////////////////////////

  ///////////// Keep the ports still being used in design, and remove
  /// unnecessary ports ///////////////////
  // Iterate over the elements in del_ports
  //std::unordered_set<std::string> keep_ports;
  //for (const auto &element : gb.del_ports) {
  //  // Check if the element is present in inst_nets or assign_nets
  //  if (gb.inst_nets.count(element) > 0 || gb.assign_nets.count(element) > 0) {
  //    // Element found, remove it from del_ports
  //    keep_ports.insert(element);
  //  }
  //}

  // Iterate over keep_ports and remove elements found in del_ports
  //for (const auto &element : keep_ports) {
  //  if (gb.del_ports.find(element) != gb.del_ports.end()) {
  //    gb.del_ports.erase(element);
  //  }
  //}

  //for (const auto &dp : gb.del_ports) {
  //  // Remove port
  //}
  ////////////////////////////////////// BLOCK END
  /////////////////////////////////////////////////////////

  //////////////////////////// Get updated module for file writting
  ///////////////////////////////////////
  // gb.mod_str

  /////////////////////////////////////////////////////////////////////////

  ////////// Remove if an output of the interface module is also an input of the
  /// same module ///////////
  // Iterate over the vector and remove pairs if the first element is found in
  // the set
  //gb.indexed_intf_ins.erase(
  //    std::remove_if(
  //        gb.indexed_intf_ins.begin(), gb.indexed_intf_ins.end(),
  //        [&io_intf](const std::pair<std::string, std::vector<int>> &pair) {
  //          return io_intf.find(pair.first) != io_intf.end();
  //        }),
  //    gb.indexed_intf_ins.end());

  // Iterate over the vector and remove pairs if the first element is found in
  // the set
  //gb.indexed_intf_outs.erase(
  //    std::remove_if(
  //        gb.indexed_intf_outs.begin(), gb.indexed_intf_outs.end(),
  //        [&io_intf](const std::pair<std::string, std::vector<int>> &pair) {
  //          return io_intf.find(pair.first) != io_intf.end();
  //        }),
  //    gb.indexed_intf_outs.end());

  // Iterate over the vector
  //for (auto it = gb.intf_outs.begin(); it != gb.intf_outs.end();) {
  //  // Check if the element exists in the unordered set
  //  if (io_intf.count(*it) > 0) {
  //    // Remove the element from the vector
  //    it = gb.intf_outs.erase(it);
  //  } else {
  //    ++it;
  //  }
  //}

  // Iterate over the vector
  //for (auto it = gb.intf_ins.begin(); it != gb.intf_ins.end();) {
  //  // Check if the element exists in the unordered set
  //  if (io_intf.count(*it) > 0) {
  //    // Remove the element from the vector
  //    it = gb.intf_ins.erase(it);
  //  } else {
  //    ++it;
  //  }
  //}
  //////////////////////////////// BLOCK END
  ///////////////////////////////////////

  /////////////////// Remove the output of clock buffers from interface IOs
  /////////////////////////
  // Remove matching elements from indexed_intf_ins
  //remove_mod_clks(gb.indexed_intf_ins, mod_clks);
  //remove_mod_clks(gb.indexed_intf_outs, mod_clks);

  // Remove elements from intf_ins
  //for (const auto &clk : gb.mod_clks) {
  //  auto it = std::find(gb.intf_ins.begin(), gb.intf_ins.end(), clk);
  //  if (it != gb.intf_ins.end()) {
  //    gb.intf_ins.erase(it);
  //  }
  //}

  // Remove elements from intf_outs
  //for (const auto &clk : gb.mod_clks) {
  //  auto it = std::find(gb.intf_outs.begin(), gb.intf_outs.end(), clk);
  //  if (it != gb.intf_outs.end()) {
  //    gb.intf_outs.erase(it);
  //  }
  //}
  //////////////////////////////////// BLOCK END
  ////////////////////////////////////////////////////////

  //std::unordered_set<std::string> ports_intf;

  /////////////////////////// Add ports to interface module
  ////////////////////////////////////////////
  //add_idx_intf_ports(intf_mod, gb.indexed_intf_ins, input, ports_intf);
  //add_idx_intf_ports(intf_mod, gb.indexed_intf_outs, output, ports_intf);
  //add_idx_intf_ports(intf_mod, gb.indexed_mod_clks, output, ports_intf);
//
  //add_intf_ports(intf_mod, gb.intf_ins, input, ports_intf);
  //add_intf_ports(intf_mod, gb.intf_outs, output, ports_intf);
  //add_intf_ports(intf_mod, gb.mod_clks, output, ports_intf);
  //add_intf_ports(intf_mod, gb.intf_inouts, inout, ports_intf);
  ///////////////////////////////////////// BLOCK END
  ////////////////////////////////////////////////////////////

  ///////////////////////// Remove instances from interface and wrapper module
  //////////////////////////////////
  //for (const auto &del_inst : gb.normal_insts) {
  //  // remove instance
  //}

  //for (const auto &del_inst : gb.normal_insts) {
  //  // remove instance
  //}

  //for (const auto &del_inst : gb.gb_insts) {
  //  // remove instance
  //}
  /////////////////////////////////////// BLOCK END
  //////////////////////////////////////////////////////////////

  ///////////////////////// Remove unnecessary ports from interface module
  ///////////////////////////////////////
  //for (const auto &port : ports_intf) {
  //  // remove port
  //}

  //for (const auto &port : gb.intf_ports) {
  //  // remove port
  //}
  ////////////////////////////////////////// BLOCK END
  ///////////////////////////////////////////////////////////

  /////////////////////////////// Add interface ports and instance to the
  /// wrapper /////////////////////////////
  
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////

  /////////////////////////////// Add original module's ports and instance to
  /// the wrapper /////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  //keep_nets(intf_mod, io_intf, gb, gb.remove_intf_nets);
  //keep_nets(top_mod, gb.top_ios, gb, gb.remove_top_nets);

  //for (const auto &el : gb.remove_intf_nets) {
  //  // remove signal
  //}

  //for (const auto &el : gb.remove_top_nets) {
  //  // remove signal
  //}
  /////////////////////////// Get interface and wrapper modules as strings
  /////////////////////////////////////////////////
  // gb.intf_mod_str
  // gb.top_mod_str

  // Generate Interface data base
  //print_out_io_primitives(intf_mod, gb);

  // Parse the contents of gb.interface_data_dump as JSON
  //nlohmann::json json_object;
  //try {
  //  json_object = nlohmann::json::parse(gb.interface_data_dump);
  //} catch (const nlohmann::json::parse_error& e) {
  //  std::cerr << "Failed to parse interface data: " << e.what() << std::endl;
  //  return 1;
  //}

  // getInstIos(json_object);
  // printInstIos();

  //for (const orig_io& entry : orig_ios)
  //{
  //  std::string io_name = entry.io_name;
  //  unsigned lsb = entry.lsb;
  //  unsigned msb = entry.msb;
  //  unsigned dir = entry.dir;
	//if (dir == dir_input) {
  //      map_inputs(json_object, io_name, "Input");
  //  } else if (dir == dir_output) {
  //       map_inputs(json_object, io_name, "Output");
  //  }
  //}

 // printPinMap();

  // Remove all analyzed modules
  

  //std::filesystem::path path(file_name);
  //std::string directory = std::filesystem::current_path().string();
  //std::string js_port_file = directory + "/" + "post_synth_ports.json";
  //std::ofstream myfile(js_port_file.c_str());
  //if (myfile.is_open())
  //{
  //    json output_array = { j_module };
  //    myfile << std::setw(4) << output_array << std::endl;
  //    myfile.close();
  //    std::cout << "Output file created at: " << js_port_file << std::endl;
  //}
  //else
  //{
  //    std::cout << "Failed to create the output file." << std::endl;
  //}

  return 0; // Status OK
}