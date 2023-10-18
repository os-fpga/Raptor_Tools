#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <map>

struct orig_io
{
    std::string io_name;
    int         lsb;
    int         msb;
    unsigned dir;
};

struct Connection {
    std::string signal;
    std::map<std::string, std::string> ports;
    std::string module;
    std::string instance;
};

int getInstIos(std::string& intf_json);
int get_io_info(std::string& mod_ios);
int map_inputs (std::string& intfJson, std::string& signalName, const std::string& dir);
void printInstIos();
void print_ios();
void printPinMap();
int update_sdc (std::string& intf_json, std::string& mod_ios, 
            std::string& user_sdc, std::string& pin_table, std::string& output_sdc);
