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
#include "Array.h"          // Make class Array available

#include "VeriId.h"         // Definitions of all identifier definition tree nodes
#include "VeriCopy.h"       // Make class VeriMapForCopy available
#include "Map.h"            // Make class Map available

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

/****************************************************************************************

                                NOTE TO THE READER:

    The following is a more advanced example to demonstrate file manipulation utility
    using APIs of 'TextBasedDesignMod' class. For a simple example, please see the other
    example application.

    * We will not change the parse tree itself, but will write out modified verilog
      of the source file using the file manipulation utility.

    * We do the following:
        1. Add a comment in the file in a specific location.
        2. Remove a particular construct from the first top level module.

    * The name of the design file and the output file are taken from the command line
      argument along with the secure directory path. If no arguments are specified,
      we default to hard-coded values.

*****************************************************************************************/


bool isimod(std::string mod)
{
    gb_constructs gb;
    for (const auto& element : gb.imods)
    {
        if (mod == element) {
            std::cout << "Mod is input : Name   " << mod << std::endl;
            return true; }
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////

int prune_verilog (const char *file_name, const char *out_file_name, const char *file_base, gb_constructs &gb)
{
#ifndef VERIFIC_LINEFILE_INCLUDES_COLUMNS
    Message::PrintLine("This application example requires the compile-flag VERIFIC_LINEFILE_INCLUDES_COLUMNS (located in util/VerificSystem.h) to be active in order to run!") ;
#else
    // The following operations will be done by this application on the file:
    // 1 . File specified in the first command line argument will be analyzed.
    // 2 . Handle for file modification utility (TextBasedDesignMod) will be created.
    // 3 . Using linefile information from analyzed parse tree, file modification utility
    //     will modify the design file (not the parse tree). We perform these modifications:
    //       (a). Add a comment attribute before the case statements to make them parallel.
    //       (b). Remove the last module item from the first top level module.
    // 4 . Modified file will be written in another file that can be specified as the second
    //     command line argument, if not specified, the original file is overwritten!

    // Analyze the design file(in verilog2k mode), if there is any error, don't proceed further!
#ifdef USE_COMREAD
    const char *def_args[] = {"analyze", file_name, "-verilog_2000"} ;
    int def_argc = 3 ;

    Command *mainline = new ComAnalyze() ;

    if(!mainline->ParseArgs(def_argc, def_args)) {
        mainline->Usage() ;
        delete mainline ;
        return 1 ;
    }

    if(!mainline->Process(0)) {
        mainline->Usage() ;
        delete mainline ;
        return 2 ;
    }
    delete mainline ;
#else
    if (!veri_file::Analyze(file_name, veri_file::VERILOG_2K /*v2k*/)) return 1 ;
#endif

    // We will perform the manipulations as said before on the first top level module of the specified file:

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

    VeriModule *moduleg = (VeriModule *)all_top_modules->GetFirst();
    std::string TM = moduleg->GetName();
    std::cout << TM << " is TP" <<std::endl;
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
        	std::cout << "Inst name is " << mod_name << std::endl;
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
        	    if (!id) cout << "NOT an ID" << endl;
        	    else printf("Got an ID\n");
                bool is_gb_cons;
                std::map<std::string, int> m_items;
        	    const char *inst_name = id->InstName() ;
                for (const auto& element : gb.gb_mods) {
                    std::string str = element.first;
                    if (str == no_param_name) {
                        m_items = element.second;
                        std::cout << str << "is gb" << std::endl;
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
                    //std::map<std::string, int> m_items = element.second;
                    if (gb.insts_visited.find(inst_name) != gb.insts_visited.end()) {
                        std::cout << inst_name << " instance visited." << std::endl;
                        continue;
                    } else {
                        std::cout << inst_name << " does not exist in the set." << std::endl;
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
                        std::cout << "ID_VERIPORTCONNECT" << std::endl;
                        formal_name = expr->NamedFormal() ;
                        formal = module_item->GetPort(formal_name) ;
                        actual = expr->GetConnection() ;
                        if (actual->GetClassId() == ID_VERICONCAT) {
                            std::cout << "got the reason" << std::endl;
                            Array *expr_arr = actual->GetExpressions();
                            unsigned i;
                            VeriExpression *pexpr;
                            FOREACH_ARRAY_ITEM(expr_arr, i, pexpr)
                            {
                                actual_id = (pexpr) ? pexpr->FullId() : 0 ;
                                actual_name = actual_id->Name();
                                std::cout << "ACN : " << actual_name << "   FN : " << formal_name << std::endl;
                            }
                        } else if (actual->GetClassId() == ID_VERIINDEXEDID) {
                            VeriIndexedId *indexed_id = static_cast<VeriIndexedId*>(actual) ;
                            unsigned port_dir = indexed_id->PortExpressionDir() ;
                            unsigned port_size = indexed_id->FindSize();
                            const char *port_name = indexed_id->GetName() ; // Get port name
                            Message::Info(indexed_id->Linefile(),"here '", port_name, "' is an indexed id in a port declaration") ;
                            std::cout << "it is indexed and dir " << port_size << std::endl;
                        }  
                        else {
                            actual_id = (actual) ? actual->FullId() : 0 ;
                            actual_name = actual_id->Name();
                            std::cout << "ACN is : " << actual_name << "  and FN is : " << formal_name << std::endl;
                            prefs.push_back(formal_name);
                            //mod->RemovePortRef(inst_name /* instance name */, formal_name /* formal port name */) ;
                            if(actual_id->Dir() == VERI_INPUT) {
                                gb.del_ports.insert(actual_name);
                                //unsigned is_removed = mod->RemovePort(actual_name /* port to be removed */) ;
                                //if (!is_removed) { 
                                //    std::cout << "Cannot remove port" << actual_name << " from the module" << std::endl ;
                                //}
                                //mod->RemovePort(actual_name);
                            	std::cout << "ACN : " << actual_name << "   is input hurrrraahhh " <<std::endl;
                            } else if(actual_id->Dir() == VERI_OUTPUT) {
                                gb.del_ports.insert(actual_name);
                                //mod->RemovePort(actual_name);
                            	std::cout << "ACN : " << actual_name << "   is output hurrraahhh  " <<std::endl;
                            } else {
                            	if (imod) {
                            		// check in gb mods for direction
                                    for (const auto& pair : m_items) {
                                        //std::cout << pair.first << "  is FN Dir is : " << pair.second << std::endl;
                                        if(pair.second) {
                                            conn_info.insert(std::make_pair(actual_name, formal_name));
                                            mod->AddPort(actual_name /* port to be added*/, VERI_INPUT /* direction*/, 0 /* data type */) ;
                                        }
                                    }
                            		} else {
                            		// check in gb mods for direction
                                    for (const auto& pair : m_items) {
                                        //std::cout << pair.first << "  is FN Dir is : " << pair.second << std::endl;
                                        if(!pair.second) {
                                            conn_info.insert(std::make_pair(actual_name, formal_name));
                                            mod->AddPort(actual_name /* port to be added*/, VERI_OUTPUT /* direction*/, 0 /* data type */) ;
                                            }
                                    }
                            		}
                            	}
                                //mod->RemovePortRef(inst_name /* instance name */, formal_name /* formal port name */) ;
                            }
        	        }
                    for (const auto& prf : prefs) {
                        //std::cout << "DEL PORT : " << element << std::endl;
                        mod->RemovePortRef(inst_name /* instance name */, prf.c_str() /* formal port name */) ;
                    }
                    //mod->RemoveInstance(inst_name /* instance to be removed*/) ;
                    inst_conns = std::make_pair(inst_name, conn_info);
                    gb.del_conns.push_back(inst_conns);
                    del_insts.push_back(inst_name);
    	        }
    	    }
        }
        for (const auto& del_inst : del_insts) {
            //std::cout << "DEL PORT : " << element << std::endl;
            mod->RemoveInstance(del_inst.c_str() /* instance to be removed*/) ;
        }
    }

    for (const auto& dp : gb.del_ports) {
        std::cout << "DEL PORT : " << dp << std::endl;
        mod->RemovePort(dp.c_str());
    }


    // Printing the elements
    for (const auto& pair : gb.del_conns) {
        std::cout << "instance Name is : " << pair.first << std::endl;
        for (const auto& mapPair : pair.second) {
            std::cout << "ACN: " << mapPair.first << ", is connected to FN: " << mapPair.second << std::endl;
        }
        std::cout << std::endl;
    }

    mod_str = mod->GetPrettyPrintedString();

     // Get the module item list of module.
    Array *m_items = new_mod->GetModuleItems() ;
    unsigned j;
    VeriModuleItem *m_item;
    std::unordered_set<std::string> del_insts_;

    FOREACH_ARRAY_ITEM(m_items, i, m_item)
    {
        if (!m_item)
            continue;
        if(m_item->IsInstantiation()) {
        	std::string mod_name = m_item->GetModuleName();
        	std::cout << "Inst name is " << mod_name << std::endl;
            std::string no_param_name;
            // reducing a correctly named parametrized module MyModule(par1=99) to MyModule
            // discuss with thierry !
            for (auto k : mod_name)
                if ('(' == k)
                    break;
                else
                    no_param_name.push_back(k);
            //bool is_gb_cons;
            //std::string str;
        	//for (const auto& element : gb.gb_mods) {
            //    str = element.first;
        	//    if (str == no_param_name) {
            //        std::cout << str << "is gb" << std::endl;
            //        is_gb_cons = true;
            //    } else {
            //        is_gb_cons = false;
            //    }
            //}
            //if (is_gb_cons) {
            //    std::cout << "is a gb const" << std::endl;
            //} else{ 
                    VeriIdDef *id ;
        	        unsigned m ;
        	        Array *insts = m_item->GetInstances();
        	        FOREACH_ARRAY_ITEM(insts, m, id) {
                        bool is_gb_cons;
                        std::string str;
        	            for (const auto& element : gb.gb_mods) {
                            str = element.first;
        	                if (str == no_param_name) {
                                std::cout << str << "is gb" << std::endl;
                                is_gb_cons = true;
                                break;
                            } else {
                                is_gb_cons = false;
                            }
                        }
        	        	if (!id) cout << "NOT an ID" << endl;
        	        	else printf("Got an ID\n");
        	        	const char *inst_name = id->InstName() ;
                        if(!is_gb_cons) {
                            std::cout << inst_name << " is being deleted" << std::endl;
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
                            for (const auto& prf : prefs) {
                                //std::cout << "DEL PORT : " << element << std::endl;
                                new_mod->RemovePortRef(inst_name /* instance name */, prf.c_str() /* formal port name */) ;
                            }
                        }
                    }
                //}
            }
        }
    
    for (const auto& del_inst : del_insts_) {
        std::cout << "DEL INST : " << del_inst << std::endl;
        new_mod->RemoveInstance(del_inst.c_str() /* instance to be removed*/) ;
    }
    std::cout << new_mod->GetPrettyPrintedString() << std::endl;
    //call function to generate wrapper

    /* /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\ *
     *                Write modified source file to a file                *
     * \/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ */
    Message::PrintLine("Writing the design to file ", out_file_name) ;

    std::ofstream out_file ;
    out_file.open(out_file_name) ;
    out_file << mod_str;

    // Remove all analyzed modules
    veri_file::RemoveAllModules() ;

#endif // #ifndef VERIFIC_LINEFILE_INCLUDES_COLUMNS

    return 0 ; // Status OK
}

