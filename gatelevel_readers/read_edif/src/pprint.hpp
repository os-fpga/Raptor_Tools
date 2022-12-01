#include <iostream>
#include <vector>
#include <tuple>
#include <map>
#include "cells_sep.hpp"
class pprint
{
private:
    int x;
    std::vector<std::tuple<
        //*net name original  */ std::string,
        /*net port reference  */ std::string,
        /*net pin number   */ std::string,
        /* net instance ref  */ std::string>>
        net_vector;
    std::vector<std::tuple<
        /*instance name original  */ std::string,
        /*Instance name renamed   */ std::string,
        /*Instance cell reference  */ std::string,
        /*Instance property lut    */ std::string,
        /*Instance property width   */ std::string,
        /* check is lut */ bool>>
        instance_vector;

public:
    void ports_print(std::vector<std::tuple<std::string, std::string, std::string, std::string>> &ports_vector);
    void instances_print(std::vector<std::tuple<
                             /*instance name original  */ std::string,
                             /*Instance name renamed   */ std::string,
                             /*Instance cell reference  */ std::string,
                             /*Instance property lut    */ std::string,
                             /*Instance property width   */ std::string,
                             /* check is lut */ bool>>
                             &instance_vector);
    void nets_print(std::map<std::string, std::vector<std::tuple<
                                              //*net name original  */ std::string,
                                              /*net port reference  */ std::string,
                                              /*net pin number   */ std::string,
                                              /* net instance ref  */ std::string>>>
                                              
                        &net_map);
    void print_linklist(struct SNode *head);
};
