#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <json.hpp>
#include <algorithm>

using json = nlohmann::json;

struct Connection {
    std::string signal;
    std::map<std::string, std::string> ports;
    std::string module;
    std::string instance;
};

struct ioInfo {
    std::string ioName;
    std::string actualName;
    std::string ioDir;
};

std::vector<Connection> connections;
std::map<std::string, std::vector<Connection>> instConns;
std::map<std::string, std::vector<ioInfo>> instIOs;
std::vector<std::string> complexPrim = {"I_DDR", "I_SERDES", "PLL", "O_DDR", "O_SERDES"};

void map_inputs (const json& data, std::string signalName, const std::string& dir)
{
    Connection conn;
    
    std::string firstInst;
    /////////////////// check input connection
    for (const auto& [instanceName, instanceInfo] : data["IO_Instances"].items())
    {
        const json& ports = instanceInfo["ports"];
        std::string modName = instanceInfo["module"];
        for (const auto& [portName, portInfo] : ports.items()) {
            std::string actualSignal = portInfo["Actual"];
            if (portInfo.contains("FUNC")) {
                std::string sigDir = portInfo["FUNC"];
                if (dir == "Input" && sigDir == "IN_DIR" && actualSignal == signalName) {
                    if (modName == "I_BUF" && portName == "I") {
                        std::string outPort = ports["O"]["Actual"];
                        signalName = outPort;
                        if (conn.signal.empty()) {
                            conn.signal = actualSignal;
                        }
                        conn.ports["O"] = ports["O"]["Actual"];
                        conn.module = modName;
                        firstInst = instanceName;
                        instConns[instanceName].push_back(conn);
                    }
                } else if (dir == "Output" && sigDir == "OUT_DIR" && actualSignal == signalName) {
                    if (modName == "O_BUFT" && portName == "O") {
                        std::string inPort = ports["I"]["Actual"];
                        signalName = inPort;
                        if (conn.signal.empty()) {
                            conn.signal = actualSignal;
                        }
                        conn.ports["I"] = ports["I"]["Actual"];
                        conn.module = modName;
                        firstInst = instanceName;
                        instConns[instanceName].push_back(conn);
                    }
                }
            }
        }
    }

    for (const auto& [instanceName, instanceInfo] : data["IO_Instances"].items())
    {
        const json& ports = instanceInfo["ports"];
        std::string modName = instanceInfo["module"];
        for (const auto& [portName, portInfo] : ports.items()) {
            std::string actualSignal = portInfo["Actual"];
            if (portInfo.contains("FUNC")) {
                std::string sigDir = portInfo["FUNC"];
                bool inputSignal;
                if(sigDir == "IN_DIR" || sigDir == "IN_CLK" || sigDir == "IN_RESET") inputSignal = true;
                if (dir == "Input" && inputSignal && actualSignal == signalName) {
                    bool found = std::find(complexPrim.begin(), complexPrim.end(), modName) != complexPrim.end();
                    if (found) {
                        std::string tempSig;
                        if (conn.signal.empty()) {
                            conn.signal = actualSignal;
                        } else {
                            tempSig = conn.signal;
                            if (conn.module == "I_BUF") {
                                instConns[firstInst].pop_back();
                                conn = Connection();
                                conn.signal = tempSig;
                            }
                        }
                        conn.ports[portName] = actualSignal;
                        conn.module = modName;
                        instConns[instanceName].push_back(conn);
                    }
                } else if (dir == "Output" && sigDir == "OUT_DIR" && actualSignal == signalName) {
                    bool found = std::find(complexPrim.begin(), complexPrim.end(), modName) != complexPrim.end();
                    if (found) {
                        std::string tempSig;
                        if (conn.signal.empty()) {
                            conn.signal = actualSignal;
                        } else {
                            tempSig = conn.signal;
                            if (conn.module == "O_BUFT") {
                                instConns[firstInst].pop_back();
                                conn = Connection();
                                conn.signal = tempSig;
                            }
                        }
                        conn.ports[portName] = actualSignal;
                        conn.module = modName;
                        instConns[instanceName].push_back(conn);
                    }
                }
            }
        }
    }
    
}

int main() {
    //std::string signalNameToFind = "D";
    std::map<std::string, std::string> orig_ios = {{"D", "Input"}, 
    {"clk", "Input"}, {"abc", "Input"}, {"clk_select", "Input"}, {"rst", "Input"}, {"Q", "Output"}, {"hh", "Output"}};

    try {
        // Read the JSON data from the "interface.json" file
        std::ifstream ifs("/home/users/behzad.mahmood/Raptor_Tools/gatelevel_readers/veri_prune/pin_map/src/interface.json");
        json jsonData = json::parse(ifs);

        // Iterate through instances and their ports
    /////////////////// check input connection
    for (const auto& [instanceName, instanceInfo] : jsonData["IO_Instances"].items())
    {
        const json& ports = instanceInfo["ports"];
        std::string modName = instanceInfo["module"];
        for (const auto& [portName, portInfo] : ports.items()) {
            std::string actualSignal = portInfo["Actual"];
            if (portInfo.contains("FUNC")) {
                std::string sigDir = portInfo["FUNC"];
                ioInfo pInfo;
                pInfo.actualName = actualSignal;
                pInfo.ioName = portName;
                pInfo.ioDir = sigDir;
                instIOs[instanceName].push_back(pInfo);
            }
        }
    }

        for (const auto& pair : orig_ios) {
            if (pair.second == "Input") {
                map_inputs(jsonData, pair.first, "Input");
            } else if (pair.second == "Output") {
                 map_inputs(jsonData, pair.first, "Output");
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    std::cout << "////////////// vector //////////" << std::endl;
    // Example: Printing connections by instance
    for (const auto& entry : instConns) {
        const std::string& instance = entry.first;
        const std::vector<Connection>& instanceConnections = entry.second;

        std::cout << "Instance: " << instance << std::endl;
        for (const auto& conn : instanceConnections) {
            std::cout << "Signal: " << conn.signal << std::endl;
            std::cout << "Module: " << conn.module << std::endl;
            // Print other fields as needed
            for (const auto& port : conn.ports) {
                std::cout << "  " << port.first << ": " << port.second << std::endl;
        }
        }
    }

    // Accessing and printing the data
    for (const auto& entry : instIOs) {
        std::cout << "Instance: " << entry.first << std::endl;
        for (const auto& io : entry.second) {
            std::cout << "  Port Name: " << io.ioName << std::endl;
            std::cout << "  Actual Name: " << io.actualName << std::endl;
            std::cout << "  IO Direction: " << io.ioDir << std::endl;
        }
    }

    return 0;
}