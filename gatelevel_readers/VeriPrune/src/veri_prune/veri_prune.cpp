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
#include "veri_tokens.h"    // Definition of port direction VERI_OUTPUT, etc ...
#include "VeriConstVal.h"   // Definitions of all constant expression tree nodes
#include "VeriScope.h"      // Symbol table of locally declared identifiers
#include "Strings.h"        // Definition of class to manipulate copy, concatenate, create etc...
#include "veri_prune.h" 

#ifdef USE_COMREAD
#include "Commands.h"
#include "ComRead.h"
#endif

#ifdef VERIFIC_NAMESPACE
using namespace Verific ;
#endif

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


////////////////////////////////////////////////////////////////////////////

int prune_verilog (const char *file_name, const char *out_file_name, const char *wrapper_file_name, const char *file_base, gb_constructs &gb)
{

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

    char *mod_str;

    //Now copy of the top level module
    VeriMapForCopy id_map_table(POINTER_HASH) ;
    char *copy_name = Strings::save("copy_", mod->Name()) ;
    VeriModuleItem *new_mod_ = mod->CopyWithName(copy_name, id_map_table, 1 /* add copied module to library containing 'mod'*/) ;
    VeriModule *new_mod = (VeriModule *)new_mod_;
    delete all_top_modules ;

    // Get the module item list of module.
    Array *items = mod->GetModuleItems() ;
    VeriModuleItem *module_item;
    unsigned i;
    FOREACH_ARRAY_ITEM(items, i, module_item)
    {
        if (!module_item)
            continue;
        std::vector<std::string> del_insts;
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
                    std::vector<std::string> prefs;
                    std::map<std::string, std::string> conn_info ;
                    std::pair<std::string, std::map<std::string, std::string>> inst_conns;
                    if (gb.insts_visited.find(inst_name) != gb.insts_visited.end()) {
                        continue;
                    } else {
                        gb.insts_visited.insert(inst_name);
                    }
        	        if (id) Message::Info(id->Linefile(),"here '", inst_name, "' is the name of an instance") ;
                    VeriIdDef *formal ;
                    VeriIdDef *actual_id ;
                    VeriExpression *actual ;
                    const char *formal_name ;
                    const char *actual_name ;
        	        VeriExpression *expr ;
        	        unsigned k ;
        	        Array *port_conn_arr = id->GetPortConnects() ;
        	        FOREACH_ARRAY_ITEM(port_conn_arr, k, expr) {
                        formal_name = expr->NamedFormal() ;
                        formal = module_item->GetPort(formal_name) ;
                        actual = expr->GetConnection() ;
                        if (actual->GetClassId() == ID_VERICONCAT) {
                            Array *expr_arr = actual->GetExpressions();
                            unsigned i;
                            VeriExpression *pexpr;
                            FOREACH_ARRAY_ITEM(expr_arr, i, pexpr)
                            {
                                actual_id = (pexpr) ? pexpr->FullId() : 0 ;
                                actual_name = actual_id->Name();
                            }
                        } else if (actual->GetClassId() == ID_VERIINDEXEDID) {
                            VeriIndexedId *indexed_id = static_cast<VeriIndexedId*>(actual) ;
                            unsigned port_dir = indexed_id->PortExpressionDir() ;
                            unsigned port_size = indexed_id->FindSize();
                            const char *port_name = indexed_id->GetName() ; // Get port name
                            Message::Info(indexed_id->Linefile(),"here '", port_name, "' is an indexed id in a port declaration") ;
                        }  
                        else {
                            actual_id = (actual) ? actual->FullId() : 0 ;
                            actual_name = actual_id->Name();
                            prefs.push_back(formal_name);
                            if(actual_id->Dir() == VERI_INPUT) {
                                gb.del_ports.insert(actual_name);
                            } else if(actual_id->Dir() == VERI_OUTPUT) {
                                gb.del_ports.insert(actual_name);
                            } else {
                            	if (imod) {
                            		// check in gb mods for direction
                                    for (const auto& pair : m_items) {
                                        if(pair.second) {
                                            conn_info.insert(std::make_pair(actual_name, formal_name));
                                            mod->AddPort(actual_name /* port to be added*/, VERI_INPUT /* direction*/, 0 /* data type */) ;
                                        }
                                    }
                            		} else {
                            		// check in gb mods for direction
                                    for (const auto& pair : m_items) {
                                        if(!pair.second) {
                                            conn_info.insert(std::make_pair(actual_name, formal_name));
                                            mod->AddPort(actual_name /* port to be added*/, VERI_OUTPUT /* direction*/, 0 /* data type */) ;
                                            }
                                    }
                            		}
                            	}
                            }
        	        }
                    for (const auto& prf : prefs) {
                        mod->RemovePortRef(inst_name /* instance name */, prf.c_str() /* formal port name */) ;
                    }
                    inst_conns = std::make_pair(inst_name, conn_info);
                    gb.del_conns.push_back(inst_conns);
                    del_insts.push_back(inst_name);
    	        }
    	    }
        }
        for (const auto& del_inst : del_insts) {
            mod->RemoveInstance(del_inst.c_str() /* instance to be removed*/) ;
        }
    }


    for (const auto& dp : gb.del_ports) {
        mod->RemovePort(dp.c_str());
    }

    // to check connections of extra ports in wrapper
    //mod->AddPort("D" /* port to be added*/, VERI_OUTPUT /* direction*/, 0 /* data type */) ;

    Array *ports = mod->GetPorts() ;
    VeriIdDef *port_id;
    unsigned b;
    FOREACH_ARRAY_ITEM(ports, b, port_id) {
        gb.top_ports.push_back(port_id->GetName());
    }

    mod_str = mod->GetPrettyPrintedString();

     // Get the module item list of module.
    Array *m_items = new_mod->GetModuleItems() ;
    unsigned j;
    VeriModuleItem *m_item;
    std::unordered_set<std::string> del_insts_;

    ///////////////////////////////////////////////////////////////////////
    FOREACH_ARRAY_ITEM(m_items, i, m_item)
    {
        if (!m_item)
            continue;
        if(m_item->IsInstantiation()) 
        {
        	std::string mod_name = m_item->GetModuleName();
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
        	Array *insts = m_item->GetInstances();
        	FOREACH_ARRAY_ITEM(insts, m, id) 
            {
                bool is_gb_cons;
                std::string str;
                std::map<std::string, int> gb_items;
        	    for (const auto& element : gb.gb_mods) {
                    str = element.first;
        	        if (str == no_param_name) {
                        gb_items = element.second;
                        is_gb_cons = true;
                        break;
                    } else {
                        is_gb_cons = false;
                    }
                }
        		const char *inst_name = id->InstName() ;
                if(!is_gb_cons) 
                {
                    del_insts_.insert(inst_name);
                    std::vector<std::string> prefs;
                    const char *formal_name ;
                    const char *actual_name ;
        		    VeriExpression *expr ;
        		    unsigned k ;
        		    Array *port_conn_arr = id->GetPortConnects() ;
        		    FOREACH_ARRAY_ITEM(port_conn_arr, k, expr) {
                        formal_name = expr->NamedFormal() ;
                        prefs.push_back(formal_name);
                    }
                }
            }
        }
    }

    // Printing the elements
    for (const auto& pair : gb.del_conns) {
        const char *inst_name = (pair.first).c_str() ;
        for (const auto& mapPair : pair.second) {
            std::string actual_name = "wrapper_wire_" + mapPair.first;
            new_mod->RemovePortRef(inst_name /* instance name */, (mapPair.second).c_str() /* formal port name */) ;
            new_mod->RemoveSignal((mapPair.first).c_str() /* signal to be removed */) ;
            new_mod->AddSignal(actual_name.c_str() /* signal to be added */, new VeriDataType(VERI_WIRE, 0, 0) /* data type */, 0 /* optional initial value*/) ;
            new_mod->AddPortRef(inst_name /* instance name */, (mapPair.second).c_str() /* formal port name */, new VeriIdRef(Strings::save(actual_name.c_str())) /* actual */) ;
            gb.wrapper_conns.push_back(mapPair.first);
        }
    }
    
    for (const auto& del_inst : del_insts_) {
        new_mod->RemoveInstance(del_inst.c_str() /* instance to be removed*/) ;
    }

    VeriModuleInstantiation *if1_inst = new_mod->AddInstance("inst1", mod->Name()) ;

    for (const auto& inst_prf : gb.top_ports) {
        // Check if port exists in the connection
        auto conn = std::find(gb.wrapper_conns.begin(), gb.wrapper_conns.end(), inst_prf);
        if (conn != gb.wrapper_conns.end()) {
            const char *add_conn = (*conn).c_str();
            std::string actual_name =  std::string("wrapper_wire_") + add_conn;
            new_mod->AddPortRef("inst1" /* instance name */, add_conn /* formal port name */, new VeriIdRef(Strings::save(actual_name.c_str())) /* actual */) ;
        } else {
            new_mod->AddPortRef("inst1" /* instance name */, inst_prf.c_str() /* formal port name */, new VeriIdRef(Strings::save(inst_prf.c_str())) /* actual */) ;
        }
    }

    char *new_mod_str = new_mod->GetPrettyPrintedString();

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
    Message::PrintLine("Writing the wrapper to file ", wrapper_file_name) ;

    std::ofstream wrapper_file;
    wrapper_file.open(wrapper_file_name);
    wrapper_file << new_mod_str;

    // Remove all analyzed modules
    veri_file::RemoveAllModules() ;

    return 0 ; // Status OK
}

