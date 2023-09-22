/*
 *
 * (c) Copyright 1999 - 2022 Verific Design Automation Inc.
 * All rights reserved.
 *
 * This source code belongs to Verific Design Automation Inc.
 * It is considered trade secret and confidential, and is not to be used
 * by parties who have not received written authorization
 * from Verific Design Automation Inc.
 *
 * Only authorized users are allowed to use, copy and modify
 * this software provided that the above copyright notice
 * remains in all copies of this software.
 *
 *
 */
#include "Array.h" // Make class Array available
#include <cstring>
#include <iostream>

#include "Map.h"          // Make class Map available
#include "VeriCopy.h"     // Make class VeriMapForCopy available
#include "VeriId.h"       // Definitions of all identifier definition tree nodes
#include "VeriTreeNode.h" // Definition of VeriTreeNode

#include "Message.h" // Make message handlers available

#include "DataBase.h" // Make (hierarchical netlist) database API available
#include "Strings.h" // Definition of class to manipulate copy, concatenate, create etc...
#include "VeriConstVal.h"   // Definitions of all constant expression tree nodes
#include "VeriExpression.h" // Definitions of all verilog expression tree nodes
#include "VeriMisc.h" // Definitions of all extraneous verilog tree nodes (ie. range, path, strength, etc...)
#include "VeriModule.h"     // Definition of a VeriModule and VeriPrimitive
#include "VeriModuleItem.h" // Definitions of all verilog module item tree nodes
#include "VeriScope.h"      // Symbol table of locally declared identifiers
#include "VeriStatement.h"  // Make VeriCaseStatement class available
#include "VeriTreeNode.h"   // Definition of VeriTreeNode
#include "VeriVisitor.h"    // For visitor patterns
#include "gb_map.h"
#include "veri_file.h" // Make verilog reader available
#include "veri_prune.h"
#include <json.hpp>
using json = nlohmann::json;

#ifdef USE_COMREAD
#include "ComRead.h"
#include "Commands.h"
#endif

#ifdef VERIFIC_NAMESPACE
using namespace Verific;
#endif

#define VERI_INOUT 329
#define VERI_INPUT 330
#define VERI_OUTPUT 346
#define VERI_WIRE 392

std::vector<orig_io> orig_ios;

std::unordered_map<int, std::string> directions = {
  {VERI_INPUT, "Input"},
  {VERI_OUTPUT, "Output"},
  {VERI_INOUT, "Inout"}
};

struct ioInfo {
    std::string ioName;
    std::string actualName;
    std::string ioDir;
};
std::map<std::string, std::vector<ioInfo>> instIOs;

void getInstIos (const json& jsonData)
{
  for (const auto& [instanceName, instanceInfo] : jsonData["IO_Instances"].items())
    {
        const json& ports = instanceInfo["ports"];
        std::string modName = instanceInfo["module"];
        for (const auto& [portName, portInfo] : ports.items()) {
            std::string actualSignal = portInfo["Actual"];
            if (portInfo.contains("FUNC")) {
                std::string sigDir = portInfo["FUNC"];
                ioInfo pInfo;
                pInfo.actualName = actualSignal;
                pInfo.ioName = portName;
                pInfo.ioDir = sigDir;
                instIOs[instanceName].push_back(pInfo);
            }
        }
    }
}

void printInstIos()
{
    for (const auto& entry : instIOs) {
        std::cout << "Instance: " << entry.first << std::endl;
        for (const auto& io : entry.second) {
            std::cout << "  Port Name: " << io.ioName << std::endl;
            std::cout << "  Actual Name: " << io.actualName << std::endl;
            std::cout << "  IO Direction: " << io.ioDir << std::endl;
        }
    }
}

std::vector<std::string> direction_print_outs = {
    "IN_DIR", "OUT_DIR", "INOUT_DIR", "OUT_CLK", "IN_CLK", "IN_RESET"};
std::unordered_map<std::string, std::map<std::string, int>>
    current_primitive_spec_map;

void keep_nets(VeriModule *mod, std::unordered_set<std::string> io_intf,
               gb_constructs &gb, std::vector<std::string> &remove_nets) {
  std::unordered_set<std::string> keep_intf_ios;
  keep_intf_ios.insert(gb.intf_ios.begin(), gb.intf_ios.end());
  keep_intf_ios.insert(io_intf.begin(), io_intf.end());
  Array *items = mod->GetModuleItems();
  VeriModuleItem *item;
  unsigned im;
  // Get the module item list of module.
  FOREACH_ARRAY_ITEM(items, im, item) {
    if (!item)
      continue;
    if (ID_VERINETDECL == item->GetClassId()) {
      VeriNetDecl *net_decl = static_cast<VeriNetDecl *>(item);
      VeriIdDef *id;
      unsigned j;
      FOREACH_ARRAY_ITEM(net_decl->GetIds(), j, id) {
        if (!id)
          continue; // null pointer check
        const char *name = id->Name();
        std::string nameString(name);
        if (keep_intf_ios.find(nameString) == keep_intf_ios.end()) {
          remove_nets.push_back(nameString);
        }
      }
    }
  }
}

void remove_mod_clks(
    std::vector<std::pair<std::string, std::vector<int>>> &indexed_data,
    std::vector<std::string> &mod_clks) {
  indexed_data.erase(
      std::remove_if(
          indexed_data.begin(), indexed_data.end(),
          [&mod_clks](const std::pair<std::string, std::vector<int>> &pair) {
            return std::find(mod_clks.begin(), mod_clks.end(), pair.first) !=
                   mod_clks.end();
          }),
      indexed_data.end());
}

void add_idx_intf_ports(
    VeriModule *intf_mod,
    std::vector<std::pair<std::string, std::vector<int>>> &indexed_intf_ios,
    unsigned dir, std::unordered_set<std::string> &ports_intf) {
  for (const auto &pair : indexed_intf_ios) {
    const auto &values = pair.second;
    unsigned msb;
    unsigned lsb;
    msb = values[0];
    lsb = values[1];
    intf_mod->AddPort(
        (pair.first).c_str(), dir,
        new VeriDataType(
            0, 0, new VeriRange(new VeriIntVal(msb), new VeriIntVal(lsb))));
    ports_intf.insert(pair.first);
  }
}

void add_intf_ports(VeriModule *intf_mod, std::vector<std::string> &intf_ios,
                    unsigned dir, std::unordered_set<std::string> &ports_intf) {
  for (const auto &port : intf_ios) {
    intf_mod->AddPort(port.c_str() /* port to be added*/, dir /* direction*/,
                      0 /* data type */);
    ports_intf.insert(port);
  }
}

void add_mod_ports(
    VeriModule *mod,
    std::vector<std::pair<std::string, std::vector<int>>> &indexed_mod_ios) {
  for (const auto &pair : indexed_mod_ios) {
    const auto &values = pair.second;
    unsigned msb;
    unsigned lsb;
    unsigned dir;
    if (values.size() == 3) {
      msb = values[0];
      lsb = values[1];
      dir = values[2];
    } else {
      msb = values[0];
      lsb = values[1];
      dir = VERI_INPUT;
    }
    mod->AddPort(
        (pair.first).c_str(), dir,
        new VeriDataType(
            0, 0, new VeriRange(new VeriIntVal(msb), new VeriIntVal(lsb))));
  }
}

void gather_data(VeriModule *mod, gb_constructs &gb, const char *formal_name,
                 std::map<std::string, int> &m_items, VeriExpression *actual) {
  std::string actual_name;
  VeriIdDef *actual_id;
  if (actual->GetIndexExpr()) {
    actual_id = (actual) ? actual->GetId() : 0;
  } else {
    actual_id = (actual) ? actual->FullId() : 0;
  }
  if (actual_id) {
    std::vector<int> io_data;
    unsigned msb = actual_id->GetMsbOfRange();
    unsigned lsb = actual_id->GetLsbOfRange();
    VeriIndexedId *indexed_id = static_cast<VeriIndexedId *>(actual);
    unsigned port_size = indexed_id->FindSize();
    actual_name = actual_id->Name();
    if (actual_id->Dir() == VERI_INPUT) {
      for (const auto &pair : m_items) {
        if (strcmp((pair.first).c_str(), formal_name) == 0) {
          if (pair.second != IN_CLK && pair.second != IN_RESET) {
            gb.del_ports.insert(actual_name);
          }
        }
      }
      gb.intf_ins.push_back(actual_name);
    } else if (actual_id->Dir() == VERI_OUTPUT) {
      gb.intf_outs.push_back(actual_name);
      gb.del_ports.insert(actual_name);
    } else if (actual_id->Dir() == VERI_INOUT) {
      gb.intf_inouts.push_back(actual_name);
      gb.del_ports.insert(actual_name);
    } else {
      // check in gb mods for direction
      for (const auto &pair : m_items) {
        if (strcmp((pair.first).c_str(), formal_name) == 0) {
          if (pair.second == OUT_DIR) {
            if (port_size > 1) {
              io_data.push_back(msb);
              io_data.push_back(lsb);
              io_data.push_back(VERI_INPUT);
              gb.indexed_mod_ios.push_back(
                  std::make_pair(actual_name, io_data));
              io_data.pop_back();
              gb.indexed_intf_outs.push_back(
                  std::make_pair(actual_name, io_data));
            } else {
              if (actual->GetIndexExpr()) {
                VeriIdDef *sig_id = mod->FindDeclared(actual_name.c_str());
                msb = sig_id->GetMsbOfRange();
                lsb = sig_id->GetLsbOfRange();
                io_data.push_back(msb);
                io_data.push_back(lsb);
                io_data.push_back(VERI_INPUT);
                gb.indexed_mod_ios.push_back(
                    std::make_pair(actual_name, io_data));
                io_data.pop_back();
                gb.indexed_intf_outs.push_back(
                    std::make_pair(actual_name, io_data));
              } else {
                gb.mod_ios.push_back(std::make_pair(actual_name, VERI_INPUT));
                gb.intf_outs.push_back(actual_name);
              }
            }
          } else if (pair.second == IN_DIR || pair.second == IN_CLK ||
                     pair.second == IN_RESET) {
            if (port_size > 1) {
              io_data.push_back(msb);
              io_data.push_back(lsb);
              io_data.push_back(VERI_OUTPUT);
              gb.indexed_mod_ios.push_back(
                  std::make_pair(actual_name, io_data));
              io_data.pop_back();
              gb.indexed_intf_ins.push_back(
                  std::make_pair(actual_name, io_data));
            } else {
              if (actual->GetIndexExpr()) {
                VeriIdDef *sig_id = mod->FindDeclared(actual_name.c_str());
                msb = sig_id->GetMsbOfRange();
                lsb = sig_id->GetLsbOfRange();
                io_data.push_back(msb);
                io_data.push_back(lsb);
                io_data.push_back(VERI_OUTPUT);
                gb.indexed_mod_ios.push_back(
                    std::make_pair(actual_name, io_data));
                io_data.pop_back();
                gb.indexed_intf_ins.push_back(
                    std::make_pair(actual_name, io_data));
              } else {
                gb.mod_ios.push_back(std::make_pair(actual_name, VERI_OUTPUT));
                gb.intf_ins.push_back(actual_name);
              }
            }
          } else if (pair.second == OUT_CLK) {
            if (port_size > 1) {
              io_data.push_back(msb);
              io_data.push_back(lsb);
              gb.indexed_mod_clks.push_back(
                  std::make_pair(actual_name, io_data));
            } else {
              if (actual->GetIndexExpr()) {
                VeriIdDef *sig_id = mod->FindDeclared(actual_name.c_str());
                msb = sig_id->GetMsbOfRange();
                lsb = sig_id->GetLsbOfRange();
                io_data.push_back(msb);
                io_data.push_back(lsb);
                gb.indexed_mod_clks.push_back(
                    std::make_pair(actual_name, io_data));
              } else {
                gb.mod_clks.push_back(actual_name);
              }
            }
          }
        }
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////

std::pair<std::string, std::string> parseKeyValue(const std::string &input) {
  size_t keyStart = input.find_first_not_of('.');
  size_t keyEnd = input.find('(');

  if (keyStart == std::string::npos || keyEnd == std::string::npos) {
    // Invalid format
    return {"", ""};
  }

  std::string key = input.substr(keyStart, keyEnd - keyStart);

  size_t valStart = keyEnd + 1;
  size_t valEnd = input.find(')', valStart);

  if (valStart == std::string::npos || valEnd == std::string::npos) {
    // Invalid format
    return {"", ""};
  }

  std::string value = input.substr(valStart, valEnd - valStart);

  return {key, value};
}

void print_out_io_primitives(VeriModule *intf_mod, gb_constructs &gb) {
    std:stringstream out_stream;
  out_stream << "{\n    \"IO_Instances\": {" << std::endl;
  Array *prim_items = intf_mod->GetModuleItems();
  VeriModuleItem *prim_item;
  unsigned idx;
  bool firstInst = true;

  FOREACH_ARRAY_ITEM(prim_items, idx, prim_item) {
    if (!prim_item)
      continue;
    if (prim_item->IsInstantiation()) {

      // out_stream << __LINE__ << " &&&& ";
      // prim_item->PrettyPrint(out_stream, 0);

      VeriIdDef *id;
      unsigned m;
      Array *insts = prim_item->GetInstances();
      FOREACH_ARRAY_ITEM(insts, m, id) {

        std::string mod_name = prim_item->GetModuleName();
        std::string no_param_name = "";

        // reducing a correctly named parametrized module MyModule(par1=99) to
        // MyModule discuss with thierry !
        for (auto k : mod_name)
          if ('(' == k)
            break;
          else
            no_param_name.push_back(k);

        std::map<std::string, int> m_items;
        const char *inst_name = id->InstName();
        if (!firstInst) {
          out_stream << ",\n";
        }
        out_stream << "        ";
        firstInst = false;
        out_stream << "\"" << inst_name << "\": {" << std::endl;
        out_stream << "            \"module\": \"" << no_param_name << "\",";

        Array *param_items = prim_item->GetParamValues();
        VeriExpression *param_assign;
        unsigned yy;
        if (param_items) {
          bool firstParam = true;
          out_stream << "\n            \"params\": {" << std::endl;
          FOREACH_ARRAY_ITEM(param_items, yy, param_assign) {
            std::stringstream ss;
            if (param_assign) {
              param_assign->PrettyPrint(ss, 0);
              auto pa = parseKeyValue(ss.str());
              if (!firstParam) {
                out_stream << ",\n";
              }
              out_stream << "                ";
              firstParam = false;
              out_stream << "\"" << pa.first << "\": ";
              if ('"' != pa.second[0])
                out_stream << "\"";
              out_stream << pa.second;
              if ('"' != pa.second.back())
                out_stream << "\"";
            }
          }
          out_stream << "\n            }," << std::endl;
        }
        Array *port_conn_arr = id->GetPortConnects();
        VeriExpression *expr;
        unsigned k;
        if (port_conn_arr) {
          bool firstPort = true;
          out_stream << "\n            \"ports\": {" << std::endl;
          FOREACH_ARRAY_ITEM(port_conn_arr, k, expr) {
            const char *formal_name = expr->NamedFormal();
            VeriExpression *actual = expr->GetConnection();
            if (actual->GetClassId() == ID_VERICONCAT) {
              VeriConcat *concat = static_cast<VeriConcat *>(actual);
              Array *expr_arr = concat->GetExpressions();
              unsigned j;
              // Iterate through all expressions
              VeriExpression *expr_;
              FOREACH_ARRAY_ITEM(expr_arr, j, expr_) {
                VeriIdDef *actual_id = expr ? expr_->GetId() : 0;
                if (actual_id){
                  if (!firstPort) {
                    out_stream << ",\n";
                  }
                  out_stream << "                \"";
                  firstPort = false;
                  out_stream << formal_name << "\": {" << std::endl;
                  if (current_primitive_spec_map.count(no_param_name) > 0 &&
                      current_primitive_spec_map[no_param_name].count(formal_name) >
                          0 &&
                      current_primitive_spec_map[no_param_name][formal_name] <
                          direction_print_outs.size()) {
                    out_stream
                        << "                    \"FUNC\": \""
                        << direction_print_outs[current_primitive_spec_map
                                                    [no_param_name][formal_name]]
                        << "\"," << std::endl;
                  }
                  out_stream << "                    \"Actual\": \"";
                  out_stream << actual_id->Name();
                  out_stream << "\"" << std::endl;
                  out_stream << "                }";
                }
              }
            } else if (actual->GetId()) {
              std::string actual_name = actual->GetId()->Name();
              if (!firstPort) {
                out_stream << ",\n";
              }
              out_stream << "                \"";
              firstPort = false;
              out_stream << formal_name << "\": {" << std::endl;
              if (current_primitive_spec_map.count(no_param_name) > 0 &&
                  current_primitive_spec_map[no_param_name].count(formal_name) >
                      0 &&
                  current_primitive_spec_map[no_param_name][formal_name] <
                      direction_print_outs.size()) {
                out_stream
                    << "                    \"FUNC\": \""
                    << direction_print_outs[current_primitive_spec_map
                                                [no_param_name][formal_name]]
                    << "\"," << std::endl;
              }
              out_stream << "                    \"Actual\": \"";
              out_stream << actual_name;
              out_stream << "\"" << std::endl;
              out_stream << "                }";
            }
          }
          out_stream << "\n            }\n";
        }
        out_stream << "        }";
      }
    }
  }
  out_stream << "\n    }\n}" << std::endl;
  gb.interface_data_dump = out_stream.str();
}

////////////////////////////////////////////////////////////////////////////

int prune_verilog(const char *file_name, gb_constructs &gb,
                  const std::string &device_name) {

  Message::SetMessageType("VERI-1116", VERIFIC_IGNORE);
  Message::SetMessageType("VERI-2541", VERIFIC_IGNORE);
  if (!veri_file::Analyze(file_name, veri_file::VERILOG_2K /*v2k*/))
    return 1;
  gb_mods_default default_mods;
#ifdef GB_CONSTRUCTS_DATA
  gb_constructs_data gb_mods_data;
#endif

  // Get all the top level modules
  Array *all_top_modules = veri_file::GetTopModules();
  // Get a handle of the first top level module, if any.
  VeriModule *mod = (all_top_modules && all_top_modules->Size())
                        ? (VeriModule *)all_top_modules->GetFirst()
                        : 0;
  if (!mod) {
    Message::PrintLine("Cannot find a top level module");
    delete all_top_modules;
    return 3;
  }

  json j_module;
  j_module["topModule"] = mod->Name();
  Array *module_ports = mod->GetPorts(); // Get the ports
  unsigned q;
  VeriIdDef *port_id_;
  FOREACH_ARRAY_ITEM(module_ports, q, port_id_) {
    if (!port_id_)
      continue;
    orig_io io_info;
    json range;
    range["msb"] = port_id_->LeftRangeBound();
    range["lsb"] = port_id_->RightRangeBound();
    j_module["ports"].push_back({{"name", port_id_->GetName()}, 
            {"direction", directions[port_id_->Dir()]}, 
            {"range", range}});
    io_info.dir = port_id_->Dir();
    io_info.io_name = port_id_->GetName();
    io_info.lsb = port_id_->RightRangeBound();
    io_info.msb = port_id_->LeftRangeBound();
    orig_ios.push_back(io_info);
  }

  // Now copy of the top level module
  VeriMapForCopy id_map_table(POINTER_HASH);
  char *intf_name = Strings::save("interface_to_fabric_", mod->Name());
  VeriModuleItem *intf_mod_ =
      mod->CopyWithName(intf_name, id_map_table,
                        1 /* add copied module to library containing 'mod'*/);
  VeriModule *intf_mod = (VeriModule *)intf_mod_;
  char *top_name = Strings::save("top_", mod->Name());
  VeriModuleItem *top_mod_ = intf_mod->CopyWithName(
      top_name, id_map_table,
      1 /* add copied module to library containing 'mod'*/);
  VeriModule *top_mod = (VeriModule *)top_mod_;
  delete all_top_modules;

  /////////////////// Rename original module ////////////////////////////
  // mod->Rename(Strings::save("nl_to_fabric_", mod->Name()));

  //////////////////////////// Remove assign statements from interface and
  /// wrapper modules /////////////////////
  // Get the module item list of module.
  Array *top_items = top_mod->GetModuleItems();
  Array *empty;
  VeriModuleItem *top_item;
  unsigned j;
  FOREACH_ARRAY_ITEM(top_items, j, top_item) {
    if (!top_item)
      continue;
    if (ID_VERICONTINUOUSASSIGN == top_item->GetClassId()) {
      top_mod->ReplaceChildBy(top_item, empty);
    }
  }

  // Get the module item list of module.
  Array *intf_items = intf_mod->GetModuleItems();
  VeriModuleItem *intf_item;
  unsigned m;
  FOREACH_ARRAY_ITEM(intf_items, m, intf_item) {
    if (!intf_item)
      continue;
    if (ID_VERIDATADECL == intf_item->GetClassId()) {
      VeriDataDecl *data_decl = static_cast<VeriDataDecl *>(intf_item);
      VeriIdDef *id;
      unsigned j;
      FOREACH_ARRAY_ITEM(data_decl->GetIds(), j, id) {
        if (!id)
          continue; // null pointer check
        const char *name = id->Name();
        gb.intf_ports.insert(name); // Store interface module's IOs
      }
    }
    if (ID_VERICONTINUOUSASSIGN == intf_item->GetClassId()) {
      intf_mod->ReplaceChildBy(intf_item, empty);
    }
  }
  /////////////////////////////////////////// BLOCK END
  //////////////////////////////////////////////////////////

  //////////////////////////////////////// Get the nets' names used in assign
  /// statements /////////////////////
  // Get the module item list of module.
  Array *items = mod->GetModuleItems();
  VeriModuleItem *module_item;
  unsigned i;
  FOREACH_ARRAY_ITEM(items, i, module_item) {
    if (!module_item)
      continue;
    if (ID_VERICONTINUOUSASSIGN == module_item->GetClassId()) {
      Array *net_assigns =
          ((VeriContinuousAssign *)module_item)->GetNetAssigns();
      VeriNetRegAssign *assign;
      unsigned j;
      FOREACH_ARRAY_ITEM(net_assigns, j, assign) {
        if (!assign)
          continue;
        if (assign->GetLValExpr()->IsConcat()) {

        } else {
          VeriExpression *lval = assign->GetLValExpr();
          if (lval->GetId()) {
            const char *lvalname = lval->GetId()->Name();
            gb.assign_nets.insert(lvalname);
          }
          VeriExpression *rval = assign->GetRValExpr();
          if (rval->IsConcat()) {
            unsigned e;
            VeriExpression *expr;
            FOREACH_ARRAY_ITEM(rval->GetExpressions(), e, expr) {
              if (expr->GetId())
                gb.assign_nets.insert(expr->GetId()->Name());
            }
          } else {
            if (rval->GetId())
              gb.assign_nets.insert(rval->GetId()->Name());
          }
        }
      }
    }
    ///////////////////////////////////////////// BLOCK END
    //////////////////////////////////////////////////
    if (module_item->IsInstantiation()) {
      std::string mod_name = module_item->GetModuleName();
      std::string no_param_name;
      // reducing a correctly named parametrized module MyModule(par1=99) to
      // MyModule discuss with thierry !
      for (auto k : mod_name)
        if ('(' == k)
          break;
        else
          no_param_name.push_back(k);

      VeriIdDef *id;
      unsigned m;
      Array *insts = module_item->GetInstances();
      FOREACH_ARRAY_ITEM(insts, m, id) {
        bool is_gb_cons;
        std::map<std::string, int> m_items;
        const char *inst_name = id->InstName();
#ifdef GB_CONSTRUCTS_DATA
        if (!gb_mods_data.device_premitives.empty() &&
            gb_mods_data.device_premitives.find(device_name) !=
                gb_mods_data.device_premitives.end()) {
          for (auto &p : gb_mods_data.device_premitives[device_name]) {
            current_primitive_spec_map[p.first] = p.second;
          }
          for (const auto &element_ :
               gb_mods_data
                   .device_premitives) { // Will update the code once I find out
                                         // how to check which device to use
            std::string device_name_ = element_.first;
            if (device_name_ == device_name) {
              for (const auto &element : element_.second) {
                std::string str = element.first;
                if (str == no_param_name) {
                  m_items = element.second;
                  is_gb_cons = true;
                  gb.contains_io_prem = true;
                  break;
                } else {
                  is_gb_cons = false;
                }
              }
            }
          }
        } else {
          for (const auto &element : default_mods.gb_mods) {
            std::string str = element.first;
            if (str == no_param_name) {
              m_items = element.second;
              is_gb_cons = true;
              gb.contains_io_prem = true;
              break;
            } else {
              is_gb_cons = false;
            }
          }
        }
#else
        for (const auto &element : default_mods.gb_mods) {
          std::string str = element.first;
          if (str == no_param_name) {
            m_items = element.second;
            is_gb_cons = true;
            gb.contains_io_prem = true;
            break;
          } else {
            is_gb_cons = false;
          }
        }
#endif
        if (is_gb_cons) {
          std::vector<std::string> prefs;
          std::unordered_map<std::string, std::vector<std::string>> del_inst;
          VeriExpression *actual;
          const char *formal_name;
          VeriExpression *expr;
          unsigned k;
          Array *port_conn_arr = id->GetPortConnects();
          FOREACH_ARRAY_ITEM(port_conn_arr, k, expr) {
            formal_name = expr->NamedFormal();
            prefs.push_back(formal_name);
            actual = expr->GetConnection();
            if (actual->GetClassId() == ID_VERICONSTVAL) {
              // Do nothing
              ;
            } else if (actual->GetClassId() == ID_VERICONCAT) {
              Array *expr_arr = actual->GetExpressions();
              unsigned c;
              VeriExpression *pexpr;
              FOREACH_ARRAY_ITEM(expr_arr, i, pexpr) {
                gather_data(mod, gb, formal_name, m_items, pexpr);
              }
            } else {
              gather_data(mod, gb, formal_name, m_items, actual);
            }
          }

          for (const auto &prf : prefs) {
            mod->RemovePortRef(inst_name /* instance name */,
                               prf.c_str() /* formal port name */);
          }
          gb.gb_insts.push_back(inst_name);
        } else {
          gb.normal_insts.push_back(inst_name);
        }
      }
    }
  }
  if (!gb.contains_io_prem)
    return 0;

  ////////////////////////////// Remove gearbox modules' instances from original
  /// module ////////////////////////////
  for (const auto &gb_inst : gb.gb_insts) {
    mod->RemoveInstance(gb_inst.c_str() /* instance to be removed*/);
  }

  ///////////////////////////// Get the nets connected to the remaining
  /// instances //////////////////////////////////
  Array *updated_items = mod->GetModuleItems();
  VeriModuleItem *updated_item;
  unsigned u;
  FOREACH_ARRAY_ITEM(updated_items, u, updated_item) {
    if (!updated_item)
      continue;
    if (updated_item->IsInstantiation()) {
      VeriIdDef *id;
      unsigned m;
      Array *insts = updated_item->GetInstances();
      FOREACH_ARRAY_ITEM(insts, m, id) {
        const char *inst_name = id->InstName();
        VeriIdDef *actual_id;
        VeriExpression *actual;
        std::string actual_name;
        VeriExpression *expr;
        unsigned k;
        Array *port_conn_arr = id->GetPortConnects();
        FOREACH_ARRAY_ITEM(port_conn_arr, k, expr) {
          actual = expr->GetConnection();
          if (actual->GetClassId() == ID_VERICONCAT) {
            VeriConcat *concat = static_cast<VeriConcat *>(actual);
            Array *expr_arr = concat->GetExpressions();
            unsigned j;
            // Iterate through all expressions
            VeriExpression *expr;
            FOREACH_ARRAY_ITEM(expr_arr, j, expr) {
              actual_id = expr ? expr->GetId() : 0;
              if (actual_id)
                gb.inst_nets.insert(actual_id->Name());
            }
          } else if (actual->GetId()) {
            actual_name = actual->GetId()->Name();
            gb.inst_nets.insert(actual_name);
          }
        }
      }
    }
  }
  //////////////////////////////// BLOCK END
  ////////////////////////////////////////////////////////////////

  //////////// Check if an output of the interface module is also an input of
  /// the same module ////////////
  std::unordered_set<std::string>
      io_intf; // To store duplicate IOs of interface module
  for (const std::string &element : gb.intf_outs) {
    if (std::find(gb.intf_ins.begin(), gb.intf_ins.end(), element) !=
        gb.intf_ins.end()) {
      io_intf.insert(element);
    }
  }

  // Iterate over indexed_intf_outs
  for (const auto &out_pair : gb.indexed_intf_outs) {
    const std::string &out_first = out_pair.first;

    // Iterate over indexed_intf_ins
    for (const auto &in_pair : gb.indexed_intf_ins) {
      const std::string &in_first = in_pair.first;

      // Compare the first elements of the pairs
      if (out_first == in_first) {
        // Push the first element to the set
        io_intf.insert(out_first);
        break; // No need to continue searching for the same out_first
      }
    }
  }

  /////////////// Remove it from the port list of the original module
  /////////////////////////////////////
  // Iterate over the vector and check if the first element exists in the set
  for (auto it = gb.indexed_mod_ios.begin(); it != gb.indexed_mod_ios.end();) {
    const std::string &firstElement = it->first;
    if (io_intf.count(firstElement) > 0) {
      it = gb.indexed_mod_ios.erase(it); // Remove the pair from the vector
    } else {
      ++it;
    }
  }

  auto remove_if_found = [&io_intf](const std::pair<std::string, int> &pair) {
    return io_intf.find(pair.first) != io_intf.end();
  };

  gb.mod_ios.erase(
      std::remove_if(gb.mod_ios.begin(), gb.mod_ios.end(), remove_if_found),
      gb.mod_ios.end());
  /////////////////////////////////////////////// BLOCK END
  //////////////////////////////////////////////////

  //////////////////////// Remove the output of the clock buffer from module IOs
  ///////////////////////////
  std::vector<std::string> mod_clks;
  for (const auto &pair : gb.indexed_mod_clks) {
    mod_clks.push_back(pair.first);
  }

  // Remove elements from indexed_mod_ios if their first element matches any in
  // mod_clks
  remove_mod_clks(gb.indexed_mod_ios, mod_clks);

  // Iterate over mod_clks
  for (const auto &clk : gb.mod_clks) {
    // Find the pair with matching first element in mod_ios
    auto it = std::find_if(gb.mod_ios.begin(), gb.mod_ios.end(),
                           [&clk](const std::pair<std::string, int> &p) {
                             return p.first == clk;
                           });

    // If a match is found, remove the pair from mod_ios
    if (it != gb.mod_ios.end()) {
      gb.mod_ios.erase(it);
    }
  }
  ////////////////////////////////// BLOCK END
  ////////////////////////////////////////////////////////////////

  ////////////////////////// Add ports to the original module
  ////////////////////////////////////////////////
  add_mod_ports(mod, gb.indexed_mod_ios);
  add_mod_ports(mod, gb.indexed_mod_clks);

  for (const auto &pair : gb.mod_ios) {
    mod->AddPort((pair.first).c_str() /* port to be added*/,
                 pair.second /* direction*/, 0 /* data type */);
  }

  for (const auto &port : gb.mod_clks) {
    mod->AddPort(port.c_str() /* port to be added*/, VERI_INPUT /* direction*/,
                 0 /* data type */);
  }
  ////////////////////////////////////// BLOCK END
  ////////////////////////////////////////////////////////////

  ///////////// Keep the ports still being used in design, and remove
  /// unnecessary ports ///////////////////
  // Iterate over the elements in del_ports
  std::unordered_set<std::string> keep_ports;
  for (const auto &element : gb.del_ports) {
    // Check if the element is present in inst_nets or assign_nets
    if (gb.inst_nets.count(element) > 0 || gb.assign_nets.count(element) > 0) {
      // Element found, remove it from del_ports
      keep_ports.insert(element);
    }
  }

  // Iterate over keep_ports and remove elements found in del_ports
  for (const auto &element : keep_ports) {
    if (gb.del_ports.find(element) != gb.del_ports.end()) {
      gb.del_ports.erase(element);
    }
  }

  for (const auto &dp : gb.del_ports) {
    mod->RemovePort(dp.c_str());
  }
  ////////////////////////////////////// BLOCK END
  /////////////////////////////////////////////////////////

  //////////////////////////// Get updated module for file writting
  ///////////////////////////////////////
  gb.mod_str = mod->GetPrettyPrintedString();

  /////////////////////////////////////////////////////////////////////////

  ////////// Remove if an output of the interface module is also an input of the
  /// same module ///////////
  // Iterate over the vector and remove pairs if the first element is found in
  // the set
  gb.indexed_intf_ins.erase(
      std::remove_if(
          gb.indexed_intf_ins.begin(), gb.indexed_intf_ins.end(),
          [&io_intf](const std::pair<std::string, std::vector<int>> &pair) {
            return io_intf.find(pair.first) != io_intf.end();
          }),
      gb.indexed_intf_ins.end());

  // Iterate over the vector and remove pairs if the first element is found in
  // the set
  gb.indexed_intf_outs.erase(
      std::remove_if(
          gb.indexed_intf_outs.begin(), gb.indexed_intf_outs.end(),
          [&io_intf](const std::pair<std::string, std::vector<int>> &pair) {
            return io_intf.find(pair.first) != io_intf.end();
          }),
      gb.indexed_intf_outs.end());

  // Iterate over the vector
  for (auto it = gb.intf_outs.begin(); it != gb.intf_outs.end();) {
    // Check if the element exists in the unordered set
    if (io_intf.count(*it) > 0) {
      // Remove the element from the vector
      it = gb.intf_outs.erase(it);
    } else {
      ++it;
    }
  }

  // Iterate over the vector
  for (auto it = gb.intf_ins.begin(); it != gb.intf_ins.end();) {
    // Check if the element exists in the unordered set
    if (io_intf.count(*it) > 0) {
      // Remove the element from the vector
      it = gb.intf_ins.erase(it);
    } else {
      ++it;
    }
  }
  //////////////////////////////// BLOCK END
  ///////////////////////////////////////

  /////////////////// Remove the output of clock buffers from interface IOs
  /////////////////////////
  // Remove matching elements from indexed_intf_ins
  remove_mod_clks(gb.indexed_intf_ins, mod_clks);
  remove_mod_clks(gb.indexed_intf_outs, mod_clks);

  // Remove elements from intf_ins
  for (const auto &clk : gb.mod_clks) {
    auto it = std::find(gb.intf_ins.begin(), gb.intf_ins.end(), clk);
    if (it != gb.intf_ins.end()) {
      gb.intf_ins.erase(it);
    }
  }

  // Remove elements from intf_outs
  for (const auto &clk : gb.mod_clks) {
    auto it = std::find(gb.intf_outs.begin(), gb.intf_outs.end(), clk);
    if (it != gb.intf_outs.end()) {
      gb.intf_outs.erase(it);
    }
  }
  //////////////////////////////////// BLOCK END
  ////////////////////////////////////////////////////////

  std::unordered_set<std::string> ports_intf;

  /////////////////////////// Add ports to interface module
  ////////////////////////////////////////////
  add_idx_intf_ports(intf_mod, gb.indexed_intf_ins, VERI_INPUT, ports_intf);
  add_idx_intf_ports(intf_mod, gb.indexed_intf_outs, VERI_OUTPUT, ports_intf);
  add_idx_intf_ports(intf_mod, gb.indexed_mod_clks, VERI_OUTPUT, ports_intf);

  add_intf_ports(intf_mod, gb.intf_ins, VERI_INPUT, ports_intf);
  add_intf_ports(intf_mod, gb.intf_outs, VERI_OUTPUT, ports_intf);
  add_intf_ports(intf_mod, gb.mod_clks, VERI_OUTPUT, ports_intf);
  add_intf_ports(intf_mod, gb.intf_inouts, VERI_INOUT, ports_intf);
  ///////////////////////////////////////// BLOCK END
  ////////////////////////////////////////////////////////////

  ///////////////////////// Remove instances from interface and wrapper module
  //////////////////////////////////
  for (const auto &del_inst : gb.normal_insts) {
    intf_mod->RemoveInstance(del_inst.c_str() /* instance to be removed*/);
  }

  for (const auto &del_inst : gb.normal_insts) {
    top_mod->RemoveInstance(del_inst.c_str() /* instance to be removed*/);
  }

  for (const auto &del_inst : gb.gb_insts) {
    top_mod->RemoveInstance(del_inst.c_str() /* instance to be removed*/);
  }
  /////////////////////////////////////// BLOCK END
  //////////////////////////////////////////////////////////////

  ///////////////////////// Remove unnecessary ports from interface module
  ///////////////////////////////////////
  for (const auto &port : ports_intf) {
    gb.intf_ports.erase(port);
  }

  for (const auto &port : gb.intf_ports) {
    intf_mod->RemovePort(port.c_str() /* port to be removed */);
  }
  ////////////////////////////////////////// BLOCK END
  ///////////////////////////////////////////////////////////

  /////////////////////////////// Add interface ports and instance to the
  /// wrapper /////////////////////////////
  Array *top_ports = intf_mod->GetPorts(); // Get the ports
  unsigned n;
  VeriIdDef *top_port_id;
  FOREACH_ARRAY_ITEM(top_ports, n, top_port_id) {
    std::unordered_set<std::string> intf_ios;
    gb.intf_ios.insert(top_port_id->GetName());
    gb.top_ports.push_back(top_port_id->GetName());
  }

  VeriModuleInstantiation *intf_inst =
      top_mod->AddInstance("intf_inst", intf_name);
  for (const auto &port : gb.top_ports) {
    top_mod->AddPortRef(
        "intf_inst" /* instance name */, port.c_str() /* formal port name */,
        new VeriIdRef(Strings::save(port.c_str())) /* actual */);
  }
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////

  /////////////////////////////// Add original module's ports and instance to
  /// the wrapper /////////////////////////////
  Array *mod_ports = mod->GetPorts(); // Get the ports
  unsigned k;
  VeriIdDef *port_id;
  FOREACH_ARRAY_ITEM(mod_ports, k, port_id) {
    if (!port_id)
      continue;
    gb.top_ios.insert(port_id->GetName());
    gb.mod_ports.push_back(port_id->GetName());
  }

  VeriModuleInstantiation *mod_inst =
      top_mod->AddInstance("mod_inst", mod->Name());
  for (const auto &port : gb.mod_ports) {
    top_mod->AddPortRef(
        "mod_inst" /* instance name */, port.c_str() /* formal port name */,
        new VeriIdRef(Strings::save(port.c_str())) /* actual */);
  }
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  keep_nets(intf_mod, io_intf, gb, gb.remove_intf_nets);
  keep_nets(top_mod, gb.top_ios, gb, gb.remove_top_nets);

  for (const auto &el : gb.remove_intf_nets) {
    intf_mod->RemoveSignal(el.c_str() /* signal to be removed */);
  }

  for (const auto &el : gb.remove_top_nets) {
    top_mod->RemoveSignal(el.c_str() /* signal to be removed */);
  }
  /////////////////////////// Get interface and wrapper modules as strings
  /////////////////////////////////////////////////
  gb.intf_mod_str = intf_mod->GetPrettyPrintedString();
  gb.top_mod_str = top_mod->GetPrettyPrintedString();

  // Generate Interface data base
  print_out_io_primitives(intf_mod, gb);

  // Parse the contents of gb.interface_data_dump as JSON
  nlohmann::json json_object;
  try {
    json_object = nlohmann::json::parse(gb.interface_data_dump);
  } catch (const nlohmann::json::parse_error& e) {
    std::cerr << "Failed to parse interface data: " << e.what() << std::endl;
    return 1;
  }

  getInstIos(json_object);
  printInstIos();

  // Remove all analyzed modules
  veri_file::RemoveAllModules();

  std::filesystem::path path(file_name);
  std::string directory = std::filesystem::current_path().string();
  std::string js_port_file = directory + "/" + "post_synth_ports.json";
  std::ofstream myfile(js_port_file.c_str());
  if (myfile.is_open())
  {
      json output_array = { j_module };
      myfile << std::setw(4) << output_array << std::endl;
      myfile.close();
      std::cout << "Output file created at: " << js_port_file << std::endl;
  }
  else
  {
      std::cout << "Failed to create the output file." << std::endl;
  }

  return 0; // Status OK
}