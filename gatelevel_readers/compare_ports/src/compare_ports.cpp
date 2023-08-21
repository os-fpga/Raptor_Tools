#include "compare_ports.h"
#include <sstream>
using namespace std;
#include <typeinfo>


stringstream rtlBuffer;
stringstream nlBuffer;

std::set<std::map<std::string, std::string>> portsInfo;

void readJsonFile(const char* fileName, stringstream& buffer) {
    ifstream fJson(fileName);
    buffer << fJson.rdbuf();
}

std::set<std::map<std::string, std::string>> process_ports(stringstream& buffer) {
    auto json = nlohmann::json::parse(buffer.str());
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
                    portsInfo.insert(portInfo);
                }
            } else{
                std::map<string, string> portInfo;
                portInfo.insert(pair<string, string>("Name", pName));
                portInfo.insert(pair<string, string>("Direction", dir));
                portsInfo.insert(portInfo);
            }
        } else {
            std::map<string, string> portInfo;
            portInfo.insert(pair<string, string>("Name", pName));
            portInfo.insert(pair<string, string>("Direction", dir));
            portsInfo.insert(portInfo);
        }
    }
    return portsInfo;
}

int compare_ports(const char* rtlPortInfo, const char* nlPortInfo) {
    readJsonFile(rtlPortInfo, rtlBuffer);
    readJsonFile(nlPortInfo, nlBuffer);
    std::set<std::map<std::string, std::string>> rtlPorts;
    std::set<std::map<std::string, std::string>> nlPorts;
    rtlPorts = process_ports(rtlBuffer) ;
    nlPorts = process_ports(nlBuffer) ;
    if(rtlPorts == nlPorts){
        printf("Pre and Post synthesis ports are same\n");
        return 0;
    } else {
        throw std::runtime_error("Error: Ports are different\n");
        return 1;
    }
}