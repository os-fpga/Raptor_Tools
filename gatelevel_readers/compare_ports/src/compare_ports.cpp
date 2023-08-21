#include "compare_ports.h"
#include <sstream>
using namespace std;
#include <typeinfo>


stringstream rtlBuffer;
stringstream nlBuffer;

std::set<std::map<std::string, std::string>> portsInfo;

void readJsonFile(const char* fileName, stringstream& buffer) {
    ifstream fJson(fileName);
    buffer.str(""); // Clear the stringstream
    buffer << fJson.rdbuf();
    fJson.close(); // Close the file stream
}

void clearPortsInfo() {
    portsInfo.clear();
}

void print_ports(const std::set<std::map<std::string, std::string>>& portsSet, const std::string& setName) {
    std::cout << setName << " Ports:\n";
    for (const auto& portMap : portsSet) {
        std::cout << "{ ";
        for (const auto& pair : portMap) {
            std::cout << pair.first << ": " << pair.second << ", ";
        }
        std::cout << "}\n";
    }
    std::cout << "\n";
}

std::set<std::map<std::string, std::string>> process_ports(stringstream& buffer) {
    clearPortsInfo();
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

void clearBuffers() {
    rtlBuffer.str(""); // Clear the stringstream
    nlBuffer.str("");  // Clear the stringstream
}

int compare_ports(const char* rtlPortInfo, const char* nlPortInfo) {
    readJsonFile(rtlPortInfo, rtlBuffer);
    readJsonFile(nlPortInfo, nlBuffer);
    std::set<std::map<std::string, std::string>> rtlPorts;
    std::set<std::map<std::string, std::string>> nlPorts;
    bool hasAdditionalPorts = false;
    rtlPorts = process_ports(rtlBuffer) ;
    nlPorts = process_ports(nlBuffer) ;

    print_ports(rtlPorts, "rtlPorts");
    print_ports(nlPorts, "nlPorts");

    if(rtlPorts == nlPorts){
        printf("Pre and Post synthesis ports are same\n");
        return 0;
    }
    // Check for missing ports from rtlPorts in nlPorts
    for (const auto& rtlPortMap : rtlPorts) {
        if (nlPorts.find(rtlPortMap) == nlPorts.end()) {
            std::cerr << "Error: Port missing after synthesis\n";
            return 1;
        }
    }

    // Check for additional ports in nlPorts and generate a warning
    for (const auto& nlPortMap : nlPorts) {
        if (rtlPorts.find(nlPortMap) == rtlPorts.end()) {
            hasAdditionalPorts = true;
            break;
        }
    }

    if (hasAdditionalPorts) {
        std::cerr << "Warning: Additional ports after synthesis\n";
    }

    clearBuffers(); // Clear the buffers after processing

    return 0;
}