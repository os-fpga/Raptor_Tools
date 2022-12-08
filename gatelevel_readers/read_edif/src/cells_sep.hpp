#ifndef CELL_SEP
#define CELL_SEP
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <tuple>
#include <map>
#include "../src/edif_parsing.hpp"
class cells_sep
{
public:
    std::tuple<
        /*Port name original  */ std::string,
        /*Port name renamed   */ std::string,
        /*Port direction      */ std::string,
        /*Port size           */ std::string>
        ports_tuple;

    std::vector<std::tuple<
        /*Port name original  */ std::string,
        /*Port name renamed   */ std::string,
        /*Port direction      */ std::string,
        /*Port size           */ std::string>>
        ports_vector;

 
    std::tuple<
        /*instance name original  */ std::string,
        /*Instance name renamed   */ std::string,
        /*Instance cell reference  */ std::string,
        /*Instance property lut    */ std::string,
        /*Instance property width   */ std::string,
        /* check is lut */ bool>
        instance_tuple;

    std::vector<std::tuple<
        /*instance name original  */ std::string,
        /*Instance name renamed   */ std::string,
        /*Instance cell reference  */ std::string,
        /*Instance property lut    */ std::string,
        /*Instance property width   */ std::string,
        /* check is lut */ bool>>
        instance_vector;

    std::tuple<
        //*net name original  */ std::string,
        /*net port reference  */ std::string,
        /*net pin number   */ std::string,
        /* net instance ref  */ std::string,
        /*net name original  */ std::string>
        net_tuple;

    std::vector<std::tuple<
        //*net name original  */ std::string,
        /*net port reference  */ std::string,
        /*net pin number   */ std::string,
        /* net instance ref  */ std::string,
        /*net name original  */ std::string>>
        net_vector;

    std::map<std::string, std::vector<std::tuple<
                              //*net name original  */ std::string,
                              /*net port reference  */ std::string,
                              /*net pin number   */ std::string,
                              /* net instance ref  */ std::string,
                                /*net name original  */ std::string>>>
        net_map;

    struct cells
    {
        std::string cell_name_orig;
        std::string cell_name_renamed;
        std::vector<std::tuple<
            /*instance name original  */ std::string,
            /*Instance name renamed   */ std::string,
            /*Instance cell reference  */ std::string,
            /*Instance property lut    */ std::string,
            /*Instance property width   */ std::string,
            /* check is lut */ bool>>
            instance_vector;
        std::vector<std::tuple<
            /*Port name original  */ std::string,
            /*Port name renamed   */ std::string,
            /*Port direction      */ std::string,
            /*Port size           */ std::string>>
            ports_vector;
        std::map<std::string, std::vector<std::tuple<
                                  //*net name original  */ std::string,
                                  /*net port reference  */ std::string,
                                  /*net pin number   */ std::string,
                                  /* net instance ref  */ std::string,
                                  /*net name original  */ std::string>>>
            net_map;
    };

    std::string top_module;
    void ports(struct SNode *head);
    void instances(struct SNode *head);
    void get_nets(struct SNode *head, std::string net_name, std::string net_name_remaned);
    void nets(struct SNode *head, std::string net_name);
    void get_cell_data(struct SNode *head, std::string cell_name, struct cells *cell_);

public:
   // bool string_compare(const std::string &f_name, const std::string &ext);
    std::vector<struct cells> cells_vector;
    void iterate(struct SNode *head);
};

#endif