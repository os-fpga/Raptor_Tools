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

#include "Map.h" // Make associated hash table class Map available
#include "Set.h" // Make associated hash table class Set available

#include "Message.h" // Make message handlers available

#include "veri_file.h"      // Make verilog reader available
#include "VeriModule.h"     // Definition of a VeriModule and VeriPrimitive
#include "VeriExpression.h" // Definitions of all verilog expression tree nodes
#include "VeriId.h"         // Definitions of all verilog identifier tree nodes
#include "VeriModuleItem.h" // Definitions of all verilog module item tree nodes
#include "VeriStatement.h"  // Definitions of all verilog statement tree nodes
#include "VeriMisc.h"       // Definitions of all extraneous verilog tree nodes (ie. range, path, strength, etc...)

#ifdef USE_COMREAD
#include "Commands.h"
#include "ComRead.h"
#endif

#include <iostream>
#include <sstream> // std::stringstream, std::stringbuf
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <ctype.h>
#include <bitset>
#include <cctype>
#include <stdexcept>
#include <boost/multiprecision/cpp_int.hpp>
#include <algorithm>
#include <regex>

#include "DataBase.h" // Make (hierarchical netlist) database API available
#include "simple_netlist.h"
#include "ieee_1735.h"
#include "RuntimeFlags.h"
#include "VeriCompileFlags.h" // Verilog-specific compile flags

#ifdef VERIFIC_NAMESPACE
using namespace Verific;
#endif

unsigned err = RuntimeFlags::SetVar("veri_preserve_user_nets", 1);

#ifdef PRODUCTION_BUILD
#include "License_manager.hpp"
#endif
using namespace boost::multiprecision;
using namespace std;
std::unordered_map<std::string, std::pair<int, int>> netBusMap;
string dirs[] = {"DIR_INOUT", "DIR_IN", "DIR_OUT", "DIR_NONE"};
void packEscaped(string &ss)
{
    unsigned int cnt = 0;
    for (unsigned int i = 0; i < ss.size(); ++i)
    {

        if (isspace(ss[i]))
            ++cnt;
        else
            ss[i - cnt] = ss[i];
    }
    while (cnt--)
        ss.pop_back();
    if ('\\' == ss[0])
    {
        ss.erase(0, 1);
    }
}

bool is_string_param_(const std::string &param)
{
    /* Empty param is considered a string */
    if (param.empty())
    {
        return true;
    }

    /* There have to be at least 2 characters (the quotes) */
    if (param.length() < 2)
    {
        return false;
    }

    /* The first and the last characters must be quotes */
    size_t len = param.length();
    if (param[0] != '"' || param[len - 1] != '"')
    {
        return false;
    }

    /* There mustn't be any other quotes except for escaped ones */
    for (size_t i = 1; i < (len - 1); ++i)
    {
        if (param[i] == '"' && param[i - 1] != '\\')
        {
            return false;
        }
    }

    /* This is a string param */
    return true;
}

bool is_binary_param_(const std::string &param)
{
    /* Must be non-empty */
    if (param.empty())
    {
        return false;
    }

    /* The string must contain only '0' and '1' */
    for (size_t i = 0; i < param.length(); ++i)
    {
        if (param[i] != '0' && param[i] != '1')
        {
            return false;
        }
    }

    /* This is a binary word param */
    return true;
}

bool is_real_param_(const std::string &param)
{
    /* Must be non-empty */
    if (param.empty())
    {
        return false;
    }

    /* The string must match the regular expression */
    static const std::regex real_number_expr("[+-]?([0-9]*\\.[0-9]+)|([0-9]+\\.[0-9]*)");
    if (!std::regex_match(param, real_number_expr))
    {
        return false;
    }

    /* This is a real number param */
    return true;
}

/*

Limit of decimal support
1024 bits
309 decimal digits
"179769313486231590772930519078902473361797697894230657273430081157732675805500963132708477322407536021120113879871393357658789768814416622492847430639474124377767893424865485276302219601246094119453082952085005768838150682342462881473913110540827237163350510684586298239947245938479716304835356329624224137215";

*/
static string mx = "179769313486231590772930519078902473361797697894230657273430081157732675805500963132708477322407536021120113879871393357658789768814416622492847430639474124377767893424865485276302219601246094119453082952085005768838150682342462881473913110540827237163350510684586298239947245938479716304835356329624224137215";
static std::map<char, string> hexDecoder = {
    {'0', "0000"},
    {'1', "0001"},
    {'2', "0010"},
    {'3', "0011"},
    {'4', "0100"},
    {'5', "0101"},
    {'6', "0110"},
    {'7', "0111"},
    {'8', "1000"},
    {'9', "1001"},
    {'A', "1010"},
    {'B', "1011"},
    {'C', "1100"},
    {'D', "1101"},
    {'E', "1110"},
    {'F', "1111"},
    {'X', "xxxx"},
    {'a', "1010"},
    {'b', "1011"},
    {'c', "1100"},
    {'d', "1101"},
    {'e', "1110"},
    {'f', "1111"},
    {'x', "xxxx"}};

string bitsOfHexaDecimal(string &s)
{
    if (!s.size())
    {
        throw(std::invalid_argument("Can't generate an hexadecimal number out of an empty string"));
    }
    for (auto d : s)
    {
        if (!isxdigit(d) && d != 'x' && d != 'X')
            throw(std::invalid_argument("Non hexadigit in hexadecimal string" + s));
    }
    string res = "";
    res.reserve(4 * s.size());
    for (auto d : s)
    {
        for (auto c : hexDecoder[d])
            res.push_back(c);
    }
    return res;
}

int1024_t bigDecimalInteger(string &s)
{
    if (!s.size())
    {
        throw(std::invalid_argument("Can't generate a decimal number out of an empty string"));
    }
    for (auto d : s)
    {
        if (!isdigit(d))
            throw(std::invalid_argument("Non digit in adecimal string" + s));
    }
    if (s.size() > mx.size() || (s.size() == mx.size() && s > mx))
    {
        throw(std::invalid_argument("Can't generate a decimal number bigger than " + mx));
    }
    int1024_t res = 0;
    for (auto d : s)
    {
        res = res * 10;
        res = res + (d - '0');
    }
    return res;
}
string bitsOfBigDecimal(string &s)
{

    string res = "";
    int1024_t v = bigDecimalInteger(s);
    int1024_t r;
    while (v)
    {
        r = v % 2;
        res.push_back(r ? '1' : '0');
        v = v / 2;
    }
    reverse(begin(res), end(res));
    return res;
}
void bits(const string &exp, std::vector<std::string> &vec_, string &strRes)
{
    std::vector<std::string> vec;
    if (exp.size() < 4)
        throw(std::invalid_argument("Not a valid expression, should be of size more than 3 " + exp));
    if (!isdigit(exp[0]))
        throw(std::invalid_argument("Not a valid expression (i.e 4'h0A9 ) " + exp));
    stringstream ss(exp);
    string rad_and_value;

    unsigned int bit_size;
    int base = 2;
    ss >> bit_size >> rad_and_value;
    string value = rad_and_value.substr(2);
    string bit_value;

    if (rad_and_value[1] == 'b' || rad_and_value[1] == 'B')
    {
        base = 2;
        bit_value = value;
        for (auto d : bit_value)
        {
            if ('1' != d && '0' != d && 'x' != d)
                throw(std::invalid_argument("Not valid bit value " + string(1, d) + " in " + exp));
        }
    }
    else if (rad_and_value[1] == 'h' || rad_and_value[1] == 'H')
    {
        base = 16;
        bit_value = bitsOfHexaDecimal(value);
    }
    else if (rad_and_value[1] == 'd' || rad_and_value[1] == 'D')
    {
        base = 10;
        bit_value = bitsOfBigDecimal(value);
    }
    else
        throw(std::invalid_argument("Invalid base indicator " + string(1, rad_and_value[1]) + " in " + exp + ", should be in {d,D,b,B,h,H}"));
    strRes = string(bit_size, '0');
    vec = vector<string>(bit_size, "0");

    for (int idx = 0; idx < bit_size && idx < bit_value.size(); ++idx)
    {
        strRes[bit_size - 1 - idx] = bit_value[bit_value.size() - 1 - idx];
        if ('x' == bit_value[bit_value.size() - 1 - idx])
            vec[bit_size - 1 - idx] = "$undef";
        else
            vec[bit_size - 1 - idx] = std::string(1, bit_value[bit_value.size() - 1 - idx]);
    }
    // expand leading x s
    int pos = 0;
    while (pos < vec.size() && "0" == vec[pos])
        ++pos;
    if (pos < vec.size() && "$undef" == vec[pos])
    {
        for (int idx = 0; idx < pos; ++idx)
        {
            vec[idx] = "$undef";
            strRes[idx] = 'x';
        }
    }
    for (int vIdx = 0; vIdx < vec.size(); vIdx++)
    {
        vec_.push_back(vec[vIdx]);
    }
}

bool bitBlast(string sig, vector<string> &res)
{
    while (sig.size() && isspace(sig.back()))
    {
        sig.pop_back();
    }
    if (!sig.size())
        return false;
    packEscaped(sig);
    if (sig.back() != ']')
    {
        if (netBusMap.find(sig) == end(netBusMap))
            res.push_back(sig);
        else
        {
            int li = netBusMap[sig].first;
            int ri = netBusMap[sig].second;
            int step = -1;
            if (li < ri)
                step = 1;
            sig.push_back('[');
            while (li != ri)
            {
                stringstream sbit;
                sbit << sig << li << "]";
                res.push_back(sbit.str());
                li += step;
            }
            stringstream sbit;
            sbit << sig << ri << "]";
            res.push_back(sbit.str());
        }
    }
    else
    {
        int pos = -1;
        int idx = sig.size() - 2;
        while (idx && sig[idx] != '[' && ((isdigit(sig[idx]) || ':' == sig[idx] || isspace(sig[idx]))))
        {
            if (':' == sig[idx])
            {
                pos = idx;
            }
            idx--;
        }
        if (pos < 0)
        {
            packEscaped(sig);
            res.push_back(sig);
        }
        else
        {
            string l = sig.substr(idx + 1, pos - idx - 1);
            string r = sig.substr(pos + 1, sig.size() - 2 - pos);
            string s = sig.substr(0, idx);
            if (!r.size() || !l.size())
            {
                string ss(sig);
                int cnt = 0;
                for (unsigned int i = 0; i < ss.size(); ++i)
                {
                    if (isspace(ss[i]))
                    {
                        cnt++;
                    }
                    else
                    {
                        ss[i - cnt] = ss[i];
                    }
                    while (cnt)
                    {
                        ss.pop_back();
                        --cnt;
                    }
                }
                packEscaped(ss);
                res.push_back(ss);
                return true;
            }
            while (s.size() && isspace(s.back()))
            {
                s.pop_back();
            }
            s.push_back('[');
            packEscaped(s);
            stringstream ss;
            ss << l;
            int li;
            ss >> li;
            stringstream rr;
            rr << r;
            int ri;
            rr >> ri;
            int step = -1;
            if (li < ri)
                step = 1;
            while (li != ri)
            {
                stringstream sbit;
                sbit << s << li << "]";
                res.push_back(sbit.str());
                li += step;
            }
            stringstream sbit;
            sbit << s << ri << "]";
            res.push_back(sbit.str());
        }
    }
    return true;
}

bool GetBitNameReferences(string ss, int l, int r, vector<string> &vec)
{
    int i = -1;
    if (l < r)
        i = 1;
    while (l != r)
    {
        stringstream s;
        s << l;
        l += i;
        string n;
        s >> n;
        vec.push_back(ss + "[" + n + "]");
    }
    stringstream s;
    s << l;
    string n;
    s >> n;
    vec.push_back(ss + "[" + n + "]");

    return true;
}

bool bitBlast(VeriExpression *port_expr, vector<string> &res)
{
    string bitdump;
    if (!port_expr)
        return false;
    if (port_expr->GetClassId() == ID_VERICONCAT)
    {
        // Gets the array of expressions
        Array *expr_arr = port_expr->GetExpressions();
        unsigned i;
        VeriExpression *expr;
        FOREACH_ARRAY_ITEM(expr_arr, i, expr)
        {
            stringstream ss;
            expr->PrettyPrint(ss, 0); // Traverse (if you like ...)
            string st = ss.str();
            if (expr->IsConstExpr())
            {
                bits(ss.str(), res, bitdump);
            }
            else
            {
                bitBlast(ss.str(), res);
            }
        }
    }
    else
    {
        if (!port_expr)
            return false;
        stringstream ss;
        port_expr->PrettyPrint(ss, 0); // Traverse (if you like ...)
        if (port_expr->IsConstExpr())
        {
            bits(ss.str(), res, bitdump);
        }
        else
            bitBlast(ss.str(), res);
    }
    return true;
}

unsigned long long veriValue(const string &exp)
{
    if (!isdigit(exp[0]))
        throw(std::invalid_argument("Not a valid expression (i.e 4'h0A9 ) " + exp));
    stringstream ss(exp);
    string dd;
    char *str, *stops;
    unsigned int aa;
    int bb = 2;
    ss >> aa >> dd;
    if (aa < 1 || aa > 64)
        throw(std::invalid_argument("Not supporting more than 64 bit values or less than one bit " + exp));
    if (dd[1] == 'b' || dd[1] == 'B')
        bb = 2;
    else if (dd[1] == 'h' || dd[1] == 'H')
        bb = 16;
    else if (dd[1] == 'd' || dd[1] == 'D')
        bb = 10;
    else
        throw(std::invalid_argument("Invalid base indicator " + string(1, dd[1]) + " in " + exp + ", should be in {d,D,b,B,h,H}"));
    str = &dd[2];
    return strtoull(str, &stops, bb);
}

std::vector<unsigned> entryTruth(unsigned long long e, unsigned long long w)
{
    std::vector<unsigned> res(w, 0);
    unsigned p = 1;
    for (unsigned int i = 0; i < w; ++i)
    {
        int k = e & p;
        if (k)
            res[w - 1 - i] = 1;
        p *= 2;
    }
    return res;
}

void simpleTruthTable(std::string tr, std::string w, std::vector<std::vector<unsigned>> &vec)
{
    if(is_binary_param(w)) w = "32'b" + w;
    if(is_binary_param(tr)) tr = "512'b" + tr;
    unsigned long long width = veriValue(w);
    string stringRes;
    vector<string> v;
    bits(tr, v, stringRes);

    for (int i = 0; i < stringRes.size(); ++i)
    {
        if ('1' == stringRes[stringRes.size() - 1 - i])
        {
            auto ent = entryTruth(i, width);
            ent.push_back(1);
            vec.push_back(ent);
        }
    }
}

int parse_verilog(const char *file_name, simple_netlist &n_l)
{
    ieee_1735 ieee_1735;
    Array files(1);
    files.Insert(file_name);

#ifdef PRODUCTION_BUILD
    License_Manager license(License_Manager::LicensedProductName::READ_VERILOG);
#endif
    Verific::veri_file::SetPragmaProtectObject(&ieee_1735);
    // Now analyze the top-level design. In case of failure return.
    if (!veri_file::AnalyzeMultipleFiles(&files, veri_file::VERILOG_2K))
    {
        // Here, design analysis failed
        return 1;
    }
    // Get name of top-level module
    const char *name = veri_file::TopModule();

    // Ok now let's elaborate this module. In case of failure return.
    if (!veri_file::Elaborate(name /*module_name*/, "work" /*work_lib*/, 0 /*parameter_values_map*/))
        return 4;

    // After elaboration, we can now delete the parse-trees (to conserve memory) since
    // we don't need them anymore.
    VeriModule *module = veri_file::GetModule(name);
    unsigned i;
    Array *module_items = module->GetModuleItems();
    VeriModuleItem *module_item;

    n_l.name = module->GetName();

    // Get a handle to the top-level netlist
    Netlist *top = Netlist::PresentDesign();
    if (!top)
    {
        Message::PrintLine("Cannot find any handle to the top-level netlist");
        return 5;
    }
    // Lets accumulate all netlist
    Set netlists(POINTER_HASH);
    top->Hierarchy(netlists, 0 /* bottom to top */);

    // Now for each netlist, print the name of all netlists along with the instantiation
    // reference count, and access some netlist information.
    Netlist *netlist;
    MapIter mi, mi2;
    SetIter si;
    SetIter si2;
    vector<Netlist *> n_vec;
    FOREACH_SET_ITEM(&netlists, si, &netlist)
    {
        n_vec.push_back(netlist);
    }

    while (n_vec.size())
    {
        auto netlist = n_vec.back();
        n_vec.pop_back();
        // Skip primitives and operators
        if (netlist->IsPrimitive() || netlist->IsOperator())
            continue;
        // Print netlist name and reference count
        Message::Msg(VERIFIC_INFO, 0, netlist->Linefile(), "netlist %s was instantiated %d time(s) by netlist %s",
                     netlist->Owner()->Name(), netlist->NumOfRefs(), top->Owner()->Name());
        // Iterate over all ports of this netlist
        const char *current_block_model = netlist->Owner()->Name();
        Port *port;
        FOREACH_PORT_OF_NETLIST(netlist, mi, port)
        {
            PortBus *portb = port->Bus();
            if (DIR_INOUT == port->GetDir())
                n_l.inout_ports.push_back(port->Name());
            else if (DIR_OUT == port->GetDir())
                n_l.out_ports.push_back(port->Name());
            else if (DIR_IN == port->GetDir())
                n_l.in_ports.push_back(port->Name());
        }
        // Iterate over all portbuses of this netlist
        PortBus *portbus;
        FOREACH_PORTBUS_OF_NETLIST(netlist, mi, portbus)
        {
            // Do Nothing ...
        }
        Net *net;
        FOREACH_NET_OF_NETLIST(netlist, mi, net)
        {
            n_l.nets.push_back(net->Name());
        }
        // Iterate over all netbuses in this netlist
        NetBus *netbus;
        FOREACH_NETBUS_OF_NETLIST(netlist, mi, netbus)
        {
            netBusMap[netbus->Name()] = {netbus->LeftIndex(), netbus->RightIndex()};
        }
        Instance *instance;
        // Iterate over all references (Instances) of this netlist
        FOREACH_REFERENCE_OF_NETLIST(netlist, si2, instance)
        {
            // Iterate over all parameters of instance
            n_l.blocks.push_back(inst());
            n_l.blocks.back().name_ = instance->Name();
            n_l.blocks.back().mod_name_ = current_block_model;
            char *param_name, *param_value;
            FOREACH_PARAMETER_OF_INST(instance, mi2, param_name, param_value)
            {
                // Do what you want with them ...
                string literal(param_value);
                vector<string> v;
                string param_v;
                bool is_valid = false;
                try
                {
                    bits(literal, v, param_v);
                }
                catch (...)
                {
                    param_v = literal;
                }
                is_valid = is_string_param_(param_v) || is_binary_param_(param_v) || is_real_param_(param_v);

                if (is_valid)
                {
                    n_l.blocks.back().params_[param_name] = param_v;
                }
                else
                {
                    // Message::Msg(VERIFIC_INFO, 0, netlist->Linefile(), "V2B:: Not Supported as eblif parameter  %s ", param_value);
                }
            }
            // Iterate over all portrefs of instance
            PortRef *portref;
            FOREACH_PORTREF_OF_INST(instance, mi2, portref)
            {
                // Do what you want with it ...
                Net *net_ = portref->GetNet();
                Port *port_ = portref->GetPort();
                n_l.blocks.back().conns_.push_back({port_->Name(), net_->Name()});
            }
            if (n_l.blocks.back().params_.find("LUT") != end(n_l.blocks.back().params_))
            {
                simpleTruthTable(n_l.blocks.back().params_["LUT"], n_l.blocks.back().params_["WIDTH"], n_l.blocks.back().truthTable_);
            }
        }
    }

    FOREACH_ARRAY_ITEM(module_items, i, module_item)
    {
        if (!module_item)
            continue;
        if (ID_VERICONTINUOUSASSIGN == module_item->GetClassId())
        { // For continuous assignments*
            VeriContinuousAssign *cont_assign = static_cast<VeriContinuousAssign *>(module_item);
            unsigned j;
            Array *net_assign_arr = cont_assign->GetNetAssigns(); // Get an array of VeriNetAssigns
            VeriNetRegAssign *net_reg_assign;
            FOREACH_ARRAY_ITEM(net_assign_arr, j, net_reg_assign)
            {
                Verific::VeriExpression *lhs, *rhs, *lrhs, *rrhs;
                lhs = net_reg_assign->GetLValExpr();
                rhs = net_reg_assign->GetRValExpr();
                if (ID_VERIBINARYOPERATOR == rhs->GetClassId())
                {
                    vector<string> l_vec;
                    bitBlast(lhs, l_vec);
                    auto lhsVal = l_vec[0];
                    packEscaped(lhsVal);
                    lrhs = rhs->GetLeft();
                    rrhs = rhs->GetRight();
                    if (!lrhs || !rrhs)
                        continue;
                    std::stringstream ssbits;
                    vector<string> r_vec;
                    bitBlast(rrhs, r_vec);
                    stringstream lutVal;
                    lrhs->PrettyPrint(lutVal, 0);
                    stringstream widthVal;
                    widthVal << r_vec.size();
                    n_l.blocks.push_back(inst());
                    n_l.blocks.back().mod_name_ = "$lut";
                    char *param_name, *param_value;
                    n_l.blocks.back().params_["LUT"] = lutVal.str();
                    n_l.blocks.back().params_["WIDTH"] = string("32'd") + widthVal.str();
                    if (r_vec.size() == 1)
                    {
                        auto b = r_vec[0];
                        b = ("1" == b) ? "$true" : ("0" == b) ? "$false"
                                                              : b;
                        n_l.blocks.back().conns_.push_back({"---", b});
                        n_l.blocks.back().conns_.push_back({"---", lhsVal});
                        if (n_l.blocks.back().params_.find("LUT") != end(n_l.blocks.back().params_))
                        {
                            if ((b == "$true") || (b == "$false"))
                                n_l.blocks.back().truthTable_.push_back({1, 1});
                            else
                                simpleTruthTable(n_l.blocks.back().params_["LUT"], n_l.blocks.back().params_["WIDTH"], n_l.blocks.back().truthTable_);
                        }
                    }
                    else
                    {
                        for (int idx = r_vec.size() - 1; idx > -1; --idx)
                        {
                            auto b = r_vec[idx];
                            b = ("1" == b) ? "$true" : ("0" == b) ? "$false"
                                                                  : b;
                            n_l.blocks.back().conns_.push_back({"---", b});
                        }
                        n_l.blocks.back().conns_.push_back({"---", lhsVal});
                        if (n_l.blocks.back().params_.find("LUT") != end(n_l.blocks.back().params_))
                        {
                            simpleTruthTable(n_l.blocks.back().params_["LUT"], n_l.blocks.back().params_["WIDTH"], n_l.blocks.back().truthTable_);
                        }
                    }
                }
                else
                {
                    vector<string> l_vec;
                    vector<string> r_vec;
                    bitBlast(lhs, l_vec);
                    bitBlast(rhs, r_vec);
                    for (unsigned idx = 0; idx < l_vec.size() && idx < r_vec.size(); ++idx)
                    {
                        n_l.blocks.push_back(inst());
                        n_l.blocks.back().mod_name_ = "$lut";
                        string first = r_vec[idx] == "0" ? "$false" : r_vec[idx];
                        first = first == "1" ? "$true" : first;
                        n_l.blocks.back().conns_.push_back({"__1", first});
                        n_l.blocks.back().conns_.push_back({"__2", l_vec[idx]});
                        n_l.blocks.back().truthTable_.push_back({1, 1});
                    }
                }
            }
        }
    }
    veri_file::RemoveAllModules();

    return 0;
}
