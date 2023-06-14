#ifndef SIMPLE_NETLIST_H
#define SIMPLE_NETLIST_H

#include <iostream>
#include <sstream> // std::stringstream, std::stringbuf
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <ctype.h>
#include <cstring> // strchr ...
#include <cctype>  // isalpha, etc ...
using namespace std;

struct inst
{
    std::string name_;
    std::string mod_name_;
    std::vector<std::pair<std::string, std::string>> conns_;
    std::unordered_map<std::string, std::string> params_;
    std::vector<std::vector<unsigned>> truthTable_;
    inst() {}
    inst(std::string name, std::string mod_name, std::vector<std::pair<std::string, std::string>> conn)
        : name_(name), mod_name_(mod_name), conns_(conn) {}
    void p_print(std::ostream &f)
    {

        f << "Instance : " << name_ << " of module " << mod_name_ << std::endl;
        for (auto &p : conns_)
        {
            f << "." << p.first << "(" << p.second << "), ";
        }
        f << std::endl;
        for (auto &p : params_)
        {
            f << p.first << " : " << p.second << endl;
        }

        if ("$lut" == mod_name_.substr(0, 4))
        {
            blif_names_print(f);
        }
    }
    void blif_names_print(std::ostream &f)
    {
        f << ".names";
        for (int idx = (int)conns_.size() - 2; idx >= 0; --idx)
        {
            string s = (conns_[idx].second == "n1") ? "$false" : conns_[idx].second;
            s = (s == "n2") ? "$true" : s;
            s = (s == "n3") ? "$undef" : s;
            if ('\\' == s[0])
                s[0] = ' ';
            f << " " << s;
        }
        f << " " << conns_.back().second << std::endl;
        for (auto &v : truthTable_)
        {
            for (unsigned i = 0; i < v.size() - 1; ++i)
            {
                f << v[i];
            }
            f << " " << v[v.size() - 1] << endl;
        }
    }
    void blif_subckt_print(std::ostream &f)
    {
        string no_param_name;
        // reducing a correctly named parametrized module MyModule(par1=99) to MyModule
        // discuss with thierry !
        for (auto k : mod_name_)
            if ('(' == k)
                break;
            else
                no_param_name.push_back(k);
        f << ".subckt " << no_param_name;
        for (auto &p : conns_)
        {
            string s = (p.second == "n1") ? "$false" : p.second;
            s = (s == "n2") ? "$true" : s;
            s = (s == "n3") ? "$undef" : s;
            f << " " << p.first << "=" << s;
        }
        f << std::endl;
        for (auto &p : params_)
        {
            // TO_DO verify that verific gives always binary otherwise hande other formats
            std::string s(p.second);
            std::size_t found = s.find('b');
            if (found == std::string::npos)
                found = s.find('B');
            if (found != std::string::npos)
            {
                for (int i = (int)found; i >= 0; i--)
                    s[i] = ' ';
            }
            while (s.size() && (s.back() == '"' || isspace(s.back())))
                s.pop_back();
            f << ".param " << p.first << " " << s << std::endl;
        }
    }
    void blif_print(std::ostream &f)
    {
        if ("$lut" == mod_name_.substr(0, 4))
        {
            blif_names_print(f);
        }
        else
        {
            blif_subckt_print(f);
        }
    }
};

struct simple_netlist
{
    void p_print(std::ostream &f)
    {
        f << "Netlist Name ----: " << name << std::endl;
        f << "Netlist in ports ----: " << std::endl;
        p_print(in_ports, f);
        f << "Netlist out ports ----: " << std::endl;
        p_print(out_ports, f);
        f << "Netlist nets ----: " << std::endl;
        p_print(nets, f);
        f << "Netlist blocks ----: " << std::endl;
        p_print(blocks, f);
    }
    void b_port_print_json(std::ostream &f)
    {
        bool first = true;
        f << "[\n  {\n\t\"ports\": [";
        for (auto &in : ports)
        {
            if ('\\' == in[0])
                in[0] = ' ';
            if (!first)
                f << ",";
            f << "\n\t  {";
            string s = "output";
            f << "\n\t\t\"direction\": \"" << ((in_set.find(in)==end(in_set)) ? "output" : "input") << "\",";
            f << "\n\t\t\"name\": \"" << in << "\",";
            f << "\n\t\t\"type\": \"WIRE\"";
            f << "\n\t  }" ;
            first = false;
        }
        f << "\n\t]," << endl;
        f << "\t\"topModule\": \"" << name << "\"" << endl;
        f << "  }\n]" << endl;
    }

    void b_port_print_separate_json(std::ostream &f)
    {
        bool first = true;
        f << "{\n\t\"inputs\": [";
        for (auto &in : in_ports)
        {
            if ('\\' == in[0])
                in[0] = ' ';
            if (!first)
                f << ",";
            f << "\n\t\t\"" << in << "\"";
            first = false;
        }
        f << "\n\t]," << endl;
        first = true;
        f << "\n\t\"outputs\": [";
        for (auto &in : out_ports)
        {
            if ('\\' == in[0])
                in[0] = ' ';
            if (!first)
                f << ",";
            f << "\n\t\t\"" << in << "\"";
            first = false;
        }
        f << "\n\t]\n}" << endl;
    }

    void b_print(std::ostream &f)
    {
        f << ".model " << name << std::endl;
        f << ".inputs";
        for (auto &in : in_ports)
        {
            if ('\\' == in[0])
                in[0] = ' ';
            f << " " << in;
        }
        f << endl;
        f << ".outputs";
        for (auto &in : out_ports)
        {
            if ('\\' == in[0])
                in[0] = ' ';
            f << " " << in;
        }
        f << endl;
        f << ".names $false" << endl;
        f << ".names $true\n1" << endl;
        f << ".names $undef" << endl;
        b_print(blocks, f);
        f << ".end" << endl;
    }
    void b_print(std::vector<inst> v, std::ostream &f)
    {
        for (auto &el : v)
        {
            el.blif_print(f);
        }
    }
    void p_print(std::vector<std::string> v, std::ostream &f)
    {
        for (auto &el : v)
        {
            f << "\t" << el;
            f << std::endl;
        }
        f << std::endl;
    }
    void p_print(std::vector<inst> v, std::ostream &f)
    {
        for (auto &el : v)
        {
            f << "\t";
            el.p_print(f);
            f << std::endl;
        }
        f << std::endl;
    }
    std::string name;
    std::vector<std::string> ports;
    std::vector<std::string> in_ports;
    std::vector<std::string> out_ports;
    std::vector<std::string> inout_ports;
    std::vector<std::string> nets;
    std::vector<inst> blocks;
    std::unordered_set<std::string> in_set;
    bool encrypted = false;

};

int parse_verilog(const char *file_name, simple_netlist &n_l, const char *key_file, const char *top_mod);

#endif
