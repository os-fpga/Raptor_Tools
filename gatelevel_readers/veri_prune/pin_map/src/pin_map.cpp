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
    bool isInput;
};

std::vector<Connection> connections;
std::vector<std::string> complexPrim = {"I_DDR", "I_SERDES", "PLL", "O_DDR", "O_SERDES"};

void map_inputs (const json& data, std::string signalName, const std::string& dir)
{
    Connection conn;
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
                        std::cout << "Out port is :: " << outPort << std::endl;
                        signalName = outPort;
                        if (conn.signal.empty()) {
                            conn.signal = actualSignal;
                        }
                        conn.ports["O"] = ports["O"]["Actual"];
                        conn.module = modName;
                        conn.instance = instanceName;
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
                    std::cout << "Found :: " << actualSignal << "   in mod ::  " << modName <<std::endl;
                    bool found = std::find(complexPrim.begin(), complexPrim.end(), modName) != complexPrim.end();
                    if (found) {
                        if (conn.signal.empty()) {
                            conn.signal = actualSignal;
                        } else {
                            actualSignal = conn.signal;
                            if (conn.module == "I_BUF") {
                                conn = Connection();
                                conn.signal = actualSignal;
                            }
                            else{ std::cout << signalName << "  is alsooo in   " << modName << std::endl;}
                        }
                        conn.ports[portName] = actualSignal;
                        //std::cout << "now sig is :: " << portName << std::endl;
                        conn.module = modName;
                        conn.instance = instanceName;
                    }
                }
            }
        }
    }
    connections.push_back(conn);
}

int main() {
    //std::string signalNameToFind = "D";
    std::map<std::string, std::string> orig_ios = {{"D", "Input"}, 
    {"clk", "Input"}, {"clk_select", "Input"}, {"rst", "Input"}, {"Q", "Output"}};

    try {
        // Read the JSON data from the "interface.json" file
        std::ifstream ifs("/home/users/behzad.mahmood/Raptor_Tools/gatelevel_readers/veri_prune/pin_map/src/interface.json");
        json jsonData = json::parse(ifs);

        for (const auto& pair : orig_ios) {
            if (pair.second == "Input") {
                std::cout << pair.first << " is an input" << std::endl;
                map_inputs(jsonData, pair.first, "Input");
            } else if (pair.second == "Output") {
                std::cout << pair.first << " is an output" << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    std::cout << "////////////// vector //////////" << std::endl;
    for (const Connection& conn : connections) {
        std::cout << "Signal: " << conn.signal << std::endl;
        std::cout << "Ports:" << std::endl;
        for (const auto& port : conn.ports) {
            std::cout << "  " << port.first << ": " << port.second << std::endl;
        }
        std::cout << "Module: " << conn.module << std::endl;
        std::cout << "Instance: " << conn.instance << std::endl;
        std::cout << "Is Input: " << (conn.isInput ? "true" : "false") << std::endl;
        std::cout << "--------------------------------------" << std::endl;
    }

    return 0;
}