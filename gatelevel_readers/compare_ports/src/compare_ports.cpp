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
        std::map<string, string> portInfo;
        string pName = port["name"].dump(4).c_str();
        //portInfo.insert(pair<string, string>("Name", port["name"].dump(4).c_str()));
        string dir = port["direction"].dump(4).c_str();
        if (dir == "input") {
            dir = "INPUT";
        } else if (dir == "output") {
            dir = "OUTPUT";
        } else if (dir == "inout") {
            dir = "INOUT";
        }
        
        if (port.contains("range")){
            int lsb = port["range"]["lsb"];
            int msb = port["range"]["msb"];
            if (lsb > msb) {
                int temp = lsb;
                lsb = msb;
                msb = temp;
            }
            std::cout << "LSB : " << lsb << "MSB : " << msb << endl;
            if (lsb != msb){
                for (int idx=lsb; idx<=msb; idx++){
                    string indexedPort = pName + "[" + to_string(idx) + "]";
                    std::cout << "IP : " << indexedPort << endl;
                    portInfo.insert(pair<string, string>("Name", indexedPort));
                    portInfo.insert(pair<string, string>("Direction", dir));
                    for (auto el : portInfo) {
            std::cout << el.first << "=oo " << el.second << '\n';
        }
                }
            } else{
                portInfo.insert(pair<string, string>("Name", pName));
                portInfo.insert(pair<string, string>("Direction", dir));
            }
            //portInfo.insert(pair<string, string>("LSB", to_string(lsb)));
            //portInfo.insert(pair<string, string>("MSB", to_string(msb)));
        } else {
            portInfo.insert(pair<string, string>("Name", pName));
            portInfo.insert(pair<string, string>("Direction", dir));
        }
        //portInfo.insert(pair<string, string>("Type", port["type"].dump(4).c_str()));
        rtlPortsInfo.insert(portInfo);
    }
    for (auto it = rtlPortsInfo.begin(); it != rtlPortsInfo.end(); ++it){
        for (auto el : *it) {
            std::cout << el.first << "=" << el.second << '\n';
        }
    }
    return rtlPortsInfo;
}

std::set<std::map<std::string, std::string>> get_nl_ports() {
    auto json = nlohmann::json::parse(nlBuffer.str());
    for (auto port : json[0]["ports"]) {
        std::map<string, string> portInfo;
        portInfo.insert(pair<string, string>("Name", port["name"].dump(4).c_str()));
        string dir = port["direction"].dump(4).c_str();
        if (dir == "input") {
            dir = "INPUT";
        } else if (dir == "output") {
            dir = "OUTPUT";
        } else if (dir == "inout") {
            dir = "INOUT";
        }
        portInfo.insert(pair<string, string>("Direction", dir));
        if (port.contains("range")){
            int lsb = port["range"]["lsb"];
            int msb = port["range"]["msb"];
            if (lsb > msb) {
                int temp = lsb;
                lsb = msb;
                msb = temp;
            }
            portInfo.insert(pair<string, string>("LSB", to_string(lsb)));
            portInfo.insert(pair<string, string>("MSB", to_string(msb)));
        }
        //portInfo.insert(pair<string, string>("Type", port["type"].dump(4).c_str()));
        nlPortsInfo.insert(portInfo);
    }
    for (auto it = nlPortsInfo.begin(); it != nlPortsInfo.end(); ++it){
        for (auto el : *it) {
            std::cout << el.first << "=" << el.second << '\n';
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
        printf("yessssssss ports are same");
        return 0;
    } else {
        printf("nooooo ports are different");
        return 1;
    }
}