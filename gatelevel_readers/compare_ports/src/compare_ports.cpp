#include "compare_ports.h"
#include <sstream>
using namespace std;
#include <typeinfo>

class PortComparisonException : public std::exception {
public:
    PortComparisonException(const std::string& message) : message(message) {}
    const char* what() const noexcept override {
        return message.c_str();
    }

private:
    std::string message;
};

stringstream rtlBuffer;
stringstream nlBuffer;

void readJsonFile(const char* fileName, stringstream& buffer) {
    ifstream fJson(fileName);
    buffer.str(""); // Clear the stringstream
    buffer << fJson.rdbuf();
    fJson.close(); // Close the file stream
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
    std::set<std::map<std::string, std::string>> portsInfo;
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

    if(rtlPorts == nlPorts){
        printf("Pre and Post synthesis ports are same\n");
        return 0;
    }
    // Check for missing ports from rtlPorts in nlPorts
    for (const auto& rtlPortMap : rtlPorts) {
        try {
             if (!rtlPortMap.empty()) {
                auto firstElement = rtlPortMap.rbegin();
                std::string key = firstElement->first;
                std::string value = firstElement->second;
                if (nlPorts.find(rtlPortMap) == nlPorts.end()) {
                    std::string errorMsg = "Error: Port '" + value + "' missing after synthesis";
                    throw PortComparisonException(errorMsg);
                }
            }
        } catch (const PortComparisonException& e) {
            std::cerr << e.what() << std::endl;
            clearBuffers();
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