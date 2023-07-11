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
#include <iostream>
#include <cstring>
#include "Array.h"          // Make class Array available

#include "VeriId.h"         // Definitions of all identifier definition tree nodes
#include "VeriCopy.h"       // Make class VeriMapForCopy available
#include "Map.h"            // Make class Map available
#include "VeriTreeNode.h"   // Definition of VeriTreeNode

#include "Message.h"        // Make message handlers available

#include "DataBase.h" // Make (hierarchical netlist) database API available
#include "VeriTreeNode.h"   // Definition of VeriTreeNode
#include "veri_file.h"      // Make verilog reader available
#include "VeriModule.h"     // Definition of a VeriModule and VeriPrimitive
#include "VeriExpression.h" // Definitions of all verilog expression tree nodes
#include "VeriModuleItem.h" // Definitions of all verilog module item tree nodes
#include "VeriMisc.h"       // Definitions of all extraneous verilog tree nodes (ie. range, path, strength, etc...)
#include "VeriStatement.h"  // Make VeriCaseStatement class available
#include "VeriVisitor.h"    // For visitor patterns
#include "VeriConstVal.h"   // Definitions of all constant expression tree nodes
#include "VeriScope.h"      // Symbol table of locally declared identifiers
#include "Strings.h"        // Definition of class to manipulate copy, concatenate, create etc...
#include "veri_prune.h" 

#define VERI_INOUT 329
#define VERI_INPUT 330
#define VERI_OUTPUT 346
#define VERI_WIRE 392
#define IN_DIR 0
#define OUT_DIR 1
#define INOUT_DIR 2

#ifdef USE_COMREAD
#include "Commands.h"
#include "ComRead.h"
#endif

#ifdef VERIFIC_NAMESPACE
using namespace Verific ;
#endif

static char* intf_mod_str = nullptr;
static char* top_mod_str = nullptr;
static char* mod_str=nullptr; 

char* GetINTFModString()
{
    return intf_mod_str;
}

char* GetTOPModString()
{
    return top_mod_str;
}

char* GetModString()
{
    return mod_str;
}

bool isimod(std::string mod)
{
    gb_constructs gb;
    for (const auto& element : gb.imods)
    {
        if (mod == element) {
            return true; }
    }
    return false;
}

bool isiomod(std::string mod)
{
    gb_constructs gb;
    for (const auto& element : gb.iomods)
    {
        if (mod == element) {
            return true; }
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////

int prune_verilog (const char *file_name, const char *out_file_name, const char *wrapper_file_name,  gb_constructs &gb)
{

    Message::SetMessageType("VERI-1116", VERIFIC_IGNORE);
    if (!veri_file::Analyze(file_name, veri_file::VERILOG_2K /*v2k*/)) return 1 ;

    // Get all the top level modules
    Array *all_top_modules = veri_file::GetTopModules() ;
    // Get a handle of the first top level module, if any.
    VeriModule *mod = (all_top_modules && all_top_modules->Size()) ? (VeriModule *)all_top_modules->GetFirst() : 0 ;
    if (!mod) {
        Message::PrintLine("Cannot find a top level module") ;
        delete all_top_modules ;
        return 3 ;
    }

    //Now copy of the top level module
    VeriMapForCopy id_map_table(POINTER_HASH) ;
    char *intf_name = Strings::save("intf_", mod->Name()) ;
    VeriModuleItem *intf_mod_ = mod->CopyWithName(intf_name, id_map_table, 1 /* add copied module to library containing 'mod'*/) ;
    VeriModule *intf_mod = (VeriModule *)intf_mod_;
    char *top_name = Strings::save("top_", mod->Name()) ;
    VeriModuleItem *top_mod_ = intf_mod->CopyWithName(top_name, id_map_table, 1 /* add copied module to library containing 'mod'*/) ;
    VeriModule *top_mod = (VeriModule *)top_mod_;
    delete all_top_modules ;

    // Get the module item list of module.
    Array *top_items = top_mod->GetModuleItems() ;
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
    Array *intf_items = intf_mod->GetModuleItems() ;
    VeriModuleItem *intf_item;
    unsigned m;
    FOREACH_ARRAY_ITEM(intf_items, m, intf_item) {
        if (!intf_item)
            continue;
        if (ID_VERICONTINUOUSASSIGN == intf_item->GetClassId()) {
            intf_mod->ReplaceChildBy(intf_item, empty);
        }
    }

    // Get the module item list of module.
    Array *items = mod->GetModuleItems() ;
    VeriModuleItem *module_item;
    unsigned i;
    FOREACH_ARRAY_ITEM(items, i, module_item)
    {
        if (!module_item)
            continue;
        if(module_item->IsInstantiation()) 
        {
        	std::string mod_name = module_item->GetModuleName();
            std::string no_param_name;
            // reducing a correctly named parametrized module MyModule(par1=99) to MyModule
            // discuss with thierry !
            for (auto k : mod_name)
                if ('(' == k)
                    break;
                else
                    no_param_name.push_back(k);

            VeriIdDef *id ;
        	unsigned m ;
        	Array *insts = module_item->GetInstances();
        	FOREACH_ARRAY_ITEM(insts, m, id) 
            {
                bool is_gb_cons;
                std::map<std::string, int> m_items;
        	    const char *inst_name = id->InstName() ;
                for (const auto& element : gb.gb_mods) {
                    std::string str = element.first;
                    if (str == no_param_name) {
                        m_items = element.second;
                        is_gb_cons = true;
                        break;
                    } else {
                        is_gb_cons = false;
                    }
                }
        	    if (is_gb_cons) 
                {
                    bool imod = isimod(no_param_name);
                    bool iomod = isiomod(no_param_name);
                    std::vector<std::string> prefs;
                    std::unordered_map<std::string, std::vector<std::string>> del_inst;
                    std::map<std::string, std::string> conn_info ;
                    std::pair<std::string, std::map<std::string, std::string>> inst_conns;
        	        if (id) Message::Info(id->Linefile(),"here '", inst_name, "' is the name of an instance") ;
                    VeriIdDef *formal ;
                    VeriIdDef *actual_id ;
                    VeriExpression *actual ;
                    const char *formal_name ;
                    std::string actual_name ;
        	        VeriExpression *expr ;
        	        unsigned k ;
        	        Array *port_conn_arr = id->GetPortConnects() ;
        	        FOREACH_ARRAY_ITEM(port_conn_arr, k, expr) {
                        formal_name = expr->NamedFormal() ;
                        formal = module_item->GetPort(formal_name) ;
                        actual = expr->GetConnection() ;
                        if (actual->GetClassId() == ID_VERICONSTVAL) {
                            // Do nothing
                            ;
                        } else {
                            if(actual->GetIndexExpr()) { 
                                actual_id = (actual) ? actual->GetId() : 0 ;
                            } else {
                                actual_id = (actual) ? actual->FullId() : 0 ;
                            }
                            if(actual_id) {
                                std::vector<int> io_data;
                                unsigned msb = actual_id->GetMsbOfRange();
                                unsigned lsb = actual_id->GetLsbOfRange();
                                VeriIndexedId *indexed_id = static_cast<VeriIndexedId*>(actual) ;
                                unsigned port_size = indexed_id->FindSize();
                                actual_name = actual_id->Name();
                                prefs.push_back(formal_name);
                                if(actual_id->Dir() == VERI_INPUT) {
                                    gb.intf_ins.push_back(actual_name);
                                    gb.del_ports.insert(actual_name);
                                } else if(actual_id->Dir() == VERI_OUTPUT) {
                                    gb.intf_outs.push_back(actual_name);
                                    gb.del_ports.insert(actual_name);
                                } else if(actual_id->Dir() == VERI_INOUT) {
                                    gb.intf_inouts.push_back(actual_name);
                                    gb.del_ports.insert(actual_name);
                                } else {
                                	if (imod) {
                                		// check in gb mods for direction
                                        for (const auto& pair : m_items) {
                                            if (strcmp((pair.first).c_str(), formal_name) == 0) {
                                                if(pair.second == OUT_DIR) {
                                                    if (port_size > 1) {
                                                            io_data.push_back(msb);
                                                            io_data.push_back(lsb);
                                                            io_data.push_back(VERI_INPUT);
                                                            gb.indexed_mod_ios.push_back(std::make_pair(actual_name, io_data));
                                                            io_data.pop_back();
                                                            gb.indexed_intf_outs.push_back(std::make_pair(actual_name, io_data));
                                                            std::cout << "SIZE OF ID : " << port_size << "  for  " << actual_name << std::endl;
                                                    } else {
                                                        if(actual->GetIndexExpr()) {
                                                            VeriIdDef *sig_id = mod->FindDeclared(actual_name.c_str()) ;
                                                            msb = sig_id->GetMsbOfRange();
                                                			lsb = sig_id->GetLsbOfRange();
                                                            io_data.push_back(msb);
                                                            io_data.push_back(lsb);
                                                            io_data.push_back(VERI_INPUT);
                                                            gb.indexed_mod_ios.push_back(std::make_pair(actual_name, io_data));
                                                            io_data.pop_back();
                                                            gb.indexed_intf_outs.push_back(std::make_pair(actual_name, io_data));
                                                        } else {
                                                            gb.mod_ios.push_back(std::make_pair(actual_name, VERI_INPUT));
                                                            gb.intf_outs.push_back(actual_name);
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                	} else if (iomod) {
                                		// check in gb mods for direction
                                        for (const auto& pair : m_items) {
                                            if (strcmp((pair.first).c_str(), formal_name) == 0) {
                                                if(pair.second == OUT_DIR) {
                                                    if (port_size > 1) {
                                                            io_data.push_back(msb);
                                                            io_data.push_back(lsb);
                                                            io_data.push_back(VERI_INPUT);
                                                            gb.indexed_mod_ios.push_back(std::make_pair(actual_name, io_data));
                                                            io_data.pop_back();
                                                            gb.indexed_intf_outs.push_back(std::make_pair(actual_name, io_data));
                                                            std::cout << "SIZE OF ID : " << port_size << "  for  " << actual_name << std::endl;
                                                    } else {
                                                        if(actual->GetIndexExpr()) {
                                                            VeriIdDef *sig_id = mod->FindDeclared(actual_name.c_str()) ;
                                                            msb = sig_id->GetMsbOfRange();
                                                			lsb = sig_id->GetLsbOfRange();
                                                            io_data.push_back(msb);
                                                            io_data.push_back(lsb);
                                                            io_data.push_back(VERI_INPUT);
                                                            gb.indexed_mod_ios.push_back(std::make_pair(actual_name, io_data));
                                                            io_data.pop_back();
                                                            gb.indexed_intf_outs.push_back(std::make_pair(actual_name, io_data));
                                                        } else {
                                                            gb.mod_ios.push_back(std::make_pair(actual_name, VERI_INPUT));
                                                            gb.intf_outs.push_back(actual_name);
                                                        }
                                                    }
                                                } else if(pair.second == IN_DIR) {
                                                    if (port_size > 1) {
                                                            io_data.push_back(msb);
                                                            io_data.push_back(lsb);
                                                            io_data.push_back(VERI_OUTPUT);
                                                            gb.indexed_mod_ios.push_back(std::make_pair(actual_name, io_data));
                                                            io_data.pop_back();
                                                            gb.indexed_intf_ins.push_back(std::make_pair(actual_name, io_data));
                                                            std::cout << "SIZE OF ID : " << port_size << "  for  " << actual_name << std::endl;
                                                    } else {
                                                        if(actual->GetIndexExpr()) {
                                                            VeriIdDef *sig_id = mod->FindDeclared(actual_name.c_str()) ;
                                                            msb = sig_id->GetMsbOfRange();
                                                			lsb = sig_id->GetLsbOfRange();
                                                            io_data.push_back(msb);
                                                            io_data.push_back(lsb);
                                                            io_data.push_back(VERI_OUTPUT);
                                                            gb.indexed_mod_ios.push_back(std::make_pair(actual_name, io_data));
                                                            io_data.pop_back();
                                                            gb.indexed_intf_ins.push_back(std::make_pair(actual_name, io_data));
                                                        } else {
                                                            gb.mod_ios.push_back(std::make_pair(actual_name, VERI_OUTPUT));
                                                            gb.intf_ins.push_back(actual_name);
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                	} else {
                                		// check in gb mods for direction
                                        for (const auto& pair : m_items) {
                                            if (strcmp((pair.first).c_str(), formal_name) == 0) {
                                                if(pair.second == IN_DIR) {
                                                    if (port_size > 1) {
                                                            io_data.push_back(msb);
                                                            io_data.push_back(lsb);
                                                            io_data.push_back(VERI_OUTPUT);
                                                            gb.indexed_mod_ios.push_back(std::make_pair(actual_name, io_data));
                                                            io_data.pop_back();
                                                            gb.indexed_intf_ins.push_back(std::make_pair(actual_name, io_data));
                                                            std::cout << "SIZE OF ID : " << port_size << "  for  " << actual_name << std::endl;
                                                    } else {
                                                        if(actual->GetIndexExpr()) {
                                                            VeriIdDef *sig_id = mod->FindDeclared(actual_name.c_str()) ;
                                                            msb = sig_id->GetMsbOfRange();
                                                			lsb = sig_id->GetLsbOfRange();
                                                            io_data.push_back(msb);
                                                            io_data.push_back(lsb);
                                                            io_data.push_back(VERI_OUTPUT);
                                                            gb.indexed_mod_ios.push_back(std::make_pair(actual_name, io_data));
                                                            io_data.pop_back();
                                                            gb.indexed_intf_ins.push_back(std::make_pair(actual_name, io_data));
                                                        } else {
                                                            gb.mod_ios.push_back(std::make_pair(actual_name, VERI_OUTPUT));
                                                            gb.intf_ins.push_back(actual_name);
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                	}
                                }
                            }
                        }
        	        }
                     
                    for (const auto& prf : prefs) {
                        mod->RemovePortRef(inst_name /* instance name */, prf.c_str() /* formal port name */) ;
                    }
                    gb.gb_insts.push_back(inst_name);
    	        } else {
                    gb.normal_insts.push_back(inst_name);
                }
    	    }
        }
    }

    std::unordered_set<std::string> io_intf;
    for (const std::string& element : gb.intf_outs) {
        if (std::find(gb.intf_ins.begin(), gb.intf_ins.end(), element) != gb.intf_ins.end()) {
            io_intf.insert(element);
        }
    }

    // Iterate over indexed_intf_outs
    for (const auto& out_pair : gb.indexed_intf_outs) {
        const std::string& out_first = out_pair.first;

        // Iterate over indexed_intf_ins
        for (const auto& in_pair : gb.indexed_intf_ins) {
            const std::string& in_first = in_pair.first;

            // Compare the first elements of the pairs
            if (out_first == in_first) {
                // Push the first element to the set
                io_intf.insert(out_first);
                break;  // No need to continue searching for the same out_first
            }
        }
    }

    // Iterate over the vector and check if the first element exists in the set
    for (auto it = gb.indexed_mod_ios.begin(); it != gb.indexed_mod_ios.end();) {
        const std::string& firstElement = it->first;
        if (io_intf.count(firstElement) > 0) {
            it = gb.indexed_mod_ios.erase(it); // Remove the pair from the vector
        } else {
            ++it;
        }
    }

    auto remove_if_found = [&io_intf](const std::pair<std::string, int>& pair) {
        return io_intf.find(pair.first) != io_intf.end();
    };

    gb.mod_ios.erase(std::remove_if(gb.mod_ios.begin(), gb.mod_ios.end(), remove_if_found), gb.mod_ios.end());

    for (const auto& gb_inst : gb.gb_insts) {
        mod->RemoveInstance(gb_inst.c_str() /* instance to be removed*/) ;
    }

    for (const auto& pair : gb.indexed_mod_ios) {
        const auto& values = pair.second;
        unsigned msb;
        unsigned lsb;
        unsigned dir;
        if (values.size() == 3) {
            msb = values.at(0);
            lsb = values.at(1);
            dir = values.at(2);
        }
        mod->AddPort((pair.first).c_str(), dir, new VeriDataType(0, 0, new VeriRange(new VeriIntVal(msb), new VeriIntVal(lsb)))) ;
    }

    for (const auto& pair : gb.mod_ios) {
        mod->AddPort((pair.first).c_str() /* port to be added*/, pair.second /* direction*/, 0 /* data type */) ;
    }

    for (const auto& dp : gb.del_ports) {
        mod->RemovePort(dp.c_str());
    }

    // to check connections of extra ports in wrapper
    //mod->AddPort("D" /* port to be added*/, VERI_OUTPUT /* direction*/, 0 /* data type */) ;
    mod_str = mod->GetPrettyPrintedString();

    /////////////////////////////////////////////////////////////////////////

    // Iterate over the vector and remove pairs if the first element is found in the set
    gb.indexed_intf_ins.erase(
        std::remove_if(gb.indexed_intf_ins.begin(), gb.indexed_intf_ins.end(),
            [&io_intf](const std::pair<std::string, std::vector<int>>& pair) {
                return io_intf.find(pair.first) != io_intf.end();
            }),
        gb.indexed_intf_ins.end()
    );
    
    // Iterate over the vector and remove pairs if the first element is found in the set
    gb.indexed_intf_outs.erase(
        std::remove_if(gb.indexed_intf_outs.begin(), gb.indexed_intf_outs.end(),
            [&io_intf](const std::pair<std::string, std::vector<int>>& pair) {
                return io_intf.find(pair.first) != io_intf.end();
            }),
        gb.indexed_intf_outs.end()
    );

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

    std::unordered_set<std::string> intf_sig_rem;

    for (const auto& pair : gb.indexed_intf_ins) {
        const auto& values = pair.second;
        unsigned msb;
        unsigned lsb;
        if (values.size() == 2) {
            msb = values.at(0);
            lsb = values.at(1);
        }
        intf_mod->AddPort((pair.first).c_str(), VERI_INPUT, new VeriDataType(0, 0, new VeriRange(new VeriIntVal(msb), new VeriIntVal(lsb)))) ;
        intf_sig_rem.insert(pair.first);
        //std::cout << "deleting :: " << (pair.first).c_str() << std::endl;
        //intf_mod->RemoveSignal((pair.first).c_str());
    }

    for (const auto& pair : gb.indexed_intf_outs) {
        const auto& values = pair.second;
        unsigned msb;
        unsigned lsb;
        if (values.size() == 2) {
            msb = values.at(0);
            lsb = values.at(1);
        }
        intf_mod->AddPort((pair.first).c_str(), VERI_OUTPUT, new VeriDataType(0, 0, new VeriRange(new VeriIntVal(msb), new VeriIntVal(lsb)))) ;
        intf_sig_rem.insert(pair.first);
        //intf_mod->RemoveSignal((pair.first).c_str());
    }

    for (const auto& port : gb.intf_ins) {
        intf_mod->AddPort(port.c_str() /* port to be added*/, VERI_INPUT /* direction*/, 0 /* data type */) ;
        intf_sig_rem.insert(port);
        //intf_mod->RemoveSignal(port.c_str());
    }

    for (const auto& port : gb.intf_outs) {
        intf_mod->AddPort(port.c_str() /* port to be added*/, VERI_OUTPUT /* direction*/, 0 /* data type */) ;
        intf_sig_rem.insert(port);
       // intf_mod->RemoveSignal(port.c_str());
    }

    for (const auto& port : gb.intf_inouts) {
        intf_mod->AddPort(port.c_str() /* port to be added*/, VERI_INOUT /* direction*/, 0 /* data type */) ;
    }

    for (const auto& del_inst : gb.normal_insts) {
        intf_mod->RemoveInstance(del_inst.c_str() /* instance to be removed*/) ;
    }

    for (const auto& del_inst : gb.normal_insts) {
        top_mod->RemoveInstance(del_inst.c_str() /* instance to be removed*/) ;
    }

    for (const auto& del_inst : gb.gb_insts) {
        top_mod->RemoveInstance(del_inst.c_str() /* instance to be removed*/) ;
    }

    //for (const auto& dn : intf_sig_rem) {
    //    std::cout << "REMOVING SIG : " << dn << std::endl;
    //}

    Array *top_ports = intf_mod->GetPorts() ; // Get the ports
    unsigned n ;
    VeriIdDef *top_port_id ;
    FOREACH_ARRAY_ITEM(top_ports, n, top_port_id) {
        gb.top_ports.push_back(top_port_id->GetName());
    }

    VeriModuleInstantiation *intf_inst = top_mod->AddInstance("intf_inst", intf_name) ;
    for (const auto& port : gb.top_ports) {
        top_mod->AddPortRef("intf_inst" /* instance name */, port.c_str() /* formal port name */, new VeriIdRef(Strings::save(port.c_str())) /* actual */) ;
    }

    Array *mod_ports = mod->GetPorts() ; // Get the ports
    unsigned k ;
    VeriIdDef *port_id ;
    FOREACH_ARRAY_ITEM(mod_ports, k, port_id) {
        gb.mod_ports.push_back(port_id->GetName());
    }

    VeriModuleInstantiation *mod_inst = top_mod->AddInstance("mod_inst", mod->Name()) ;
    for (const auto& port : gb.mod_ports) {
        top_mod->AddPortRef("mod_inst" /* instance name */, port.c_str() /* formal port name */, new VeriIdRef(Strings::save(port.c_str())) /* actual */) ;
    }

    intf_mod_str = intf_mod->GetPrettyPrintedString();
    top_mod_str = top_mod->GetPrettyPrintedString();

    //call function to generate wrapper

    /* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\ *
     *                Write modified source file to a file                *
     * \/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
    Message::PrintLine("Writing the design to file ", out_file_name) ;

    std::ofstream out_file ;
    out_file.open(out_file_name) ;
    out_file << mod_str;

    /* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\ *
     *                Write modified source file to a file                *
     * \/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
    //Message::PrintLine("Writing the wrapper to file ", wrapper_file_name) ;
//
    std::ofstream wrapper_file;
    wrapper_file.open(wrapper_file_name);
    wrapper_file << intf_mod_str;
    wrapper_file << top_mod_str;

    // Remove all analyzed modules
    veri_file::RemoveAllModules() ;

    return 0 ; // Status OK
}

