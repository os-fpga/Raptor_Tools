#include "compare_ports.h"
#include <sstream>
using namespace std;
#include <typeinfo>


stringstream rtlBuffer;
stringstream nlBuffer;

std::set<std::map<std::string, std::string>> rtlPortsInfo;
std::set<std::map<std::string, std::string>> nlPortsInfo;

void port_info(char* portInfo) {
    ifstream fJson(portInfo);
    rtlBuffer << fJson.rdbuf();
}

void hier_info(char* hierInfo) {
    ifstream fJson(hierInfo);
    nlBuffer << fJson.rdbuf();
}
std::set<std::map<std::string, std::string>> get_rtl_ports() {
    auto json = nlohmann::json::parse(rtlBuffer.str());
    for (auto port : json[0]["ports"]) {
        string pName = port["name"];
        string dir = port["direction"];
        if (dir == "input") {
            dir = "Input";
        } else if (dir == "output") {
            dir = "Output";
        } else if (dir == "inout") {
            dir = "Inout";
        }
        
        if (port.contains("range")){
            int lsb = port["range"]["lsb"];
            int msb = port["range"]["msb"];
            if (lsb > msb) {
                int temp = lsb;
                lsb = msb;
                msb = temp;
            }
            if (lsb != msb){
                for (int idx=lsb; idx<=msb; idx++){
                    std::map<string, string> portInfo;
                    string indexedPort = pName + "[" + to_string(idx) + "]";
                    portInfo.insert(pair<string, string>("Name", indexedPort));
                    portInfo.insert(pair<string, string>("Direction", dir));
                    rtlPortsInfo.insert(portInfo);
                }
            } else{
                std::map<string, string> portInfo;
                portInfo.insert(pair<string, string>("Name", pName));
                portInfo.insert(pair<string, string>("Direction", dir));
                rtlPortsInfo.insert(portInfo);
            }
        } else {
            std::map<string, string> portInfo;
            portInfo.insert(pair<string, string>("Name", pName));
            portInfo.insert(pair<string, string>("Direction", dir));
            rtlPortsInfo.insert(portInfo);
        }
    }
    return rtlPortsInfo;
}

std::set<std::map<std::string, std::string>> get_nl_ports() {
    auto json = nlohmann::json::parse(nlBuffer.str());
    for (auto port : json[0]["ports"]) {
        string pName = port["name"];
        string dir = port["direction"];
        if (dir == "input") {
            dir = "Input";
        } else if (dir == "output") {
            dir = "Output";
        } else if (dir == "inout") {
            dir = "Inout";
        }
        
        if (port.contains("range")){
            int lsb = port["range"]["lsb"];
            int msb = port["range"]["msb"];
            if (lsb > msb) {
                int temp = lsb;
                lsb = msb;
                msb = temp;
            }
            if (lsb != msb){
                for (int idx=lsb; idx<=msb; idx++){
                    std::map<string, string> portInfo;
                    string indexedPort = pName + "[" + to_string(idx) + "]";
                    portInfo.insert(pair<string, string>("Name", indexedPort));
                    portInfo.insert(pair<string, string>("Direction", dir));
                    nlPortsInfo.insert(portInfo);
                }
            } else{
                std::map<string, string> portInfo;
                portInfo.insert(pair<string, string>("Name", pName));
                portInfo.insert(pair<string, string>("Direction", dir));
                nlPortsInfo.insert(portInfo);
            }
        } else {
            std::map<string, string> portInfo;
            portInfo.insert(pair<string, string>("Name", pName));
            portInfo.insert(pair<string, string>("Direction", dir));
            nlPortsInfo.insert(portInfo);
        }
    }
    return nlPortsInfo;
}

int compare_ports(char* portInfo, char* hierInfo) {
    port_info(portInfo);
    hier_info(hierInfo);
    std::set<std::map<std::string, std::string>> rtlPorts;
    std::set<std::map<std::string, std::string>> nlPorts;
    rtlPorts = get_rtl_ports();
    nlPorts = get_nl_ports();
    if(rtlPorts == nlPorts){
        printf("Pre and Post synthesis ports are same\n");
        return 0;
    } else {
        throw std::runtime_error("Error: Ports are different\n");
        return 1;
    }
}