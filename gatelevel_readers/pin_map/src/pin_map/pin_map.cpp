#include "pin_map.h"
#include <json.hpp>
#include <regex>
using json = nlohmann::json;
#define VERI_INOUT 329
#define VERI_INPUT 330
#define VERI_OUTPUT 346
#define VERI_WIRE 392

std::vector<orig_io> orig_ios;                      // Information of original module's IOs
std::map<std::string, std::vector<Connection>> instConns;   // map of instance name and 
                                        // its connection with original IOs and fabric side signals
std::map<std::string, std::string> pinsMap;         // Define pinsMap at a global scope
std::vector<std::string> used_pins;
std::unordered_map<std::string, int> directions = {
  {"Input", VERI_INPUT},
  {"Output", VERI_OUTPUT},
  {"Inout", VERI_INOUT}
};
struct ioInfo {
    std::string ioName;
    std::string actualName;
    std::string ioDir;
    unsigned lsb;
    unsigned msb;
};
std::map<std::string, std::vector<ioInfo>> instIOs;

int getInstIos(std::string& intf_json)
{
  	std::ifstream jsonFile(intf_json);

    // Check if the file was opened successfully.
    if (!jsonFile.is_open()) {
        std::cerr << "Failed to open the JSON file." << std::endl;
        return 1;
    }

    // Parse the JSON data from the file.
    nlohmann::json jsonData;
    jsonFile >> jsonData;
  for (const auto& [instanceName, instanceInfo] : jsonData["IO_Instances"].items())
    {
        const json& ports = instanceInfo["ports"];
        std::string modName = instanceInfo["module"];
        for (const auto& [portName, portInfo] : ports.items()) {
            std::string actualSignal = portInfo["Actual"];
            if (portInfo.contains("FUNC")) {
                std::string sigDir = portInfo["FUNC"];
                unsigned lsb = portInfo["lsb"];
                unsigned msb = portInfo["msb"];
                ioInfo pInfo;
                pInfo.actualName = actualSignal;
                pInfo.ioName = portName;
                pInfo.ioDir = sigDir;
                pInfo.lsb = lsb;
                pInfo.msb = msb;
                instIOs[instanceName].push_back(pInfo);
            }
        }
    }
	return 0;
}

void printInstIos()
{
    for (const auto& entry : instIOs) {
        std::cout << "Instance: " << entry.first << std::endl;
        for (const auto& io : entry.second) {
            std::cout << "  Port Name: " << io.ioName << std::endl;
            std::cout << "  Actual Name: " << io.actualName << std::endl;
            std::cout << "  IO Direction: " << io.ioDir << std::endl;
            std::cout << "  LSB :  " << io.lsb << std::endl;
            std::cout << "  MSB :  " << io.msb << std::endl;
        }
    }
}

int get_io_info(std::string& mod_ios)
{
    std::ifstream jsonFile(mod_ios);

    // Check if the file was opened successfully.
    if (!jsonFile.is_open()) {
        std::cerr << "Failed to open the JSON file." << std::endl;
        return 1;
    }

    // Parse the JSON data from the file.
    json parsedJson;
    jsonFile >> parsedJson;

    // Close the file after parsing.
    jsonFile.close();

    json portsArray = parsedJson[0]["ports"];

    for (const auto& port : portsArray) {
        orig_io io_info;
        std::string dir = port["direction"];
        io_info.dir = directions[dir];
        io_info.io_name = port["name"];
        io_info.lsb = port["range"]["lsb"];
        io_info.msb = port["range"]["msb"];
        orig_ios.push_back(io_info);
    }

    return 0;
}

int map_signal (std::string& intfJson, std::string& signalName, const std::string& dir)
{
    Connection conn;
    
    std::string firstInst;
    std::ifstream jsonFile(intfJson);

    // Check if the file was opened successfully.
    if (!jsonFile.is_open()) {
        std::cerr << "Failed to open the JSON file." << std::endl;
        return 1;
    }

    // Parse the JSON data from the file.
    nlohmann::json data;
    jsonFile >> data;
    /////////////////// check input connection
    // Loop through all the instances of interface module
    for (const auto& [instanceName, instanceInfo] : data["IO_Instances"].items())
    {
        // ports contains ports information for the instance in iteration
        const json& ports = instanceInfo["ports"];
        std::string modName = instanceInfo["module"];
        // Iterate through all the ports of the instance
        for (const auto& [portName, portInfo] : ports.items()) {
            // Actual signal in the port connection
            std::string actualSignal = portInfo["Actual"];
            if (portInfo.contains("FUNC")) {
                std::string sigDir = portInfo["FUNC"];
                if (dir == "Input" && sigDir == "IN_DIR" && actualSignal == signalName) {
					bool input_buf = false;
					if(modName == "I_BUF" || modName == "CLK_BUF") input_buf = true;
                    if (input_buf && portName == "I") {
                        // For input buffers the signal connected with O is on the inner side
                        std::string outPort = ports["O"]["Actual"];
                        signalName = outPort;
                        if (conn.signal.empty()) {
                            conn.signal = actualSignal;
                        }
                        conn.ports["O"] = ports["O"]["Actual"];
                        conn.module = modName;
                        firstInst = instanceName;   // IBUFS are the first instances connected with Inputs
                        instConns[instanceName].push_back(conn);
                    }
                } else if (dir == "Output" && sigDir == "OUT_DIR" && actualSignal == signalName) {
					bool output_buf = false;
					if(modName == "O_BUF" || modName == "O_BUFT") output_buf = true;
                    if (output_buf && portName == "O") {
                        // For output buffers the signal connected with I is on the inner side
                       std::string inPort = ports["I"]["Actual"];
                        signalName = inPort;
                        if (conn.signal.empty()) {
                            conn.signal = actualSignal;
                        }
                        conn.ports["I"] = ports["I"]["Actual"];
                        conn.module = modName;
                        firstInst = instanceName;   // OBUFS are the first instances connected with Outputs
                        instConns[instanceName].push_back(conn);
                    }
                }
            }
        }
    }

    // Next iteration to check if any SERDES or DDR is connected
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
                    bool complexPrim = true;    // It is a complex (not a buf) primitive
					if (modName.find("BUF") != std::string::npos) complexPrim = false;
                    if (complexPrim) {
                        std::string tempSig;
                        if (conn.signal.empty()) {
                            conn.signal = actualSignal;
                        } else {
                            tempSig = conn.signal;  // intermediate bw buf and complex prim
                            if (conn.module == "I_BUF" || conn.module == "CLK_BUF") {
                                instConns[firstInst].pop_back();    // remove the info saved for buffer
                                conn = Connection();
                                conn.signal = tempSig;
                            }
                        }
                        // Update connection using complex prim info and push to instConns
                        conn.ports[portName] = actualSignal;
                        conn.module = modName;
                        instConns[instanceName].push_back(conn);
                    }
                } else if (dir == "Output" && sigDir == "OUT_DIR" && actualSignal == signalName) {
                    bool complexPrim = true;
					if (modName.find("BUF") != std::string::npos) complexPrim = false;
                    if (complexPrim) {
                        std::string tempSig;
                        if (conn.signal.empty()) {
                            conn.signal = actualSignal;
                        } else {
                            tempSig = conn.signal;
							bool output_buf = false;
							if(conn.module == "O_BUF" || conn.module == "O_BUFT") output_buf = true;
                            if (output_buf) {
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
    return 0;
}

void printPinMap()
{
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
}

void print_ios()
{
    for (const orig_io& entry : orig_ios)
    {
        std::string io_name = entry.io_name;
        unsigned lsb = entry.lsb;
        unsigned msb = entry.msb;
        unsigned dir = entry.dir;
        std::cout << "io_name: " << io_name << " lsb: " << lsb << " msb: " << msb << " dir: " << dir << std::endl;
    }
}

// Function to map Ball ID to Customer Name
std::map<std::string, std::string> BallID_to_CustomerName(const std::string& pin_table) {
    // Create a map to store the mapping of Ball ID to Customer Name
    std::map<std::string, std::string> ballIdToCustomer;

    // Open the CSV file
    std::ifstream file(pin_table);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open the CSV file." << std::endl;
        return ballIdToCustomer; // Return an empty map on error
    }

    // Read the CSV file line by line
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        std::string ballId;
        std::string customerName;

        // Split the line into tokens using a comma as the delimiter
        std::getline(iss, token, ','); // Bump/Pin Name
        std::getline(iss, token, ','); // Customer Name
        std::getline(iss, token, ','); // Customer Name
        customerName = token;
        std::getline(iss, token, ','); // Ball ID
        ballId = token;
        // Store the mapping of Ball ID to Customer Name in the map
        ballIdToCustomer[ballId] = customerName;
    }

    return ballIdToCustomer;
}

int write_sdc(const std::string& map_json, const std::string& pin_table, const std::string& output_sdc) {
    // Map of pin names in primitive modules and pin table
    pinsMap = {
        {"BITSLIP_ADJ", "f2g_rx_bitslip_adj"},
        {"FIFO_RST", "f2g_rx_sfifo_reset_A"},
        {"RST", "f2g_trx_reset_n_A"},
        {"DLY_ADJ", "f2g_trx_dly_adj"},
        {"DLY_INCDEC", "f2g_trx_dly_inc"},
        {"DLY_LOAD", "f2g_trx_dly_ld"},
        {"DATA_VALID", "g2f_rx_dvalid_A"},
        {"DPA_ERROR", "g2f_rx_dpa_error"},
        {"DPA_LOCK", "g2f_rx_dpa_lock"},
        {"PLL_FAST_CLK", "fast_clk"},
        {"CLK_OUT", "f2g_rx_core_clk"},
        {"LOAD_WORD", "f2g_tx_dvalid_A"},
        {"OE", "f2g_tx_oe_A"},
        {"O", "f2g_tx_clk_en_A"}
    };

    // Open the file for reading
    std::ifstream file(map_json);

    if (!file.is_open()) {
        std::cerr << "Error opening the file." << std::endl;
        return 1;
    }

    // Get the mapping of Ball ID to Customer Name
    std::map<std::string, std::string> ballIdToCustomer = BallID_to_CustomerName(pin_table);

    // Open the output SDC file for appending
    std::ofstream sdcFile(output_sdc);

    if (!sdcFile.is_open()) {
        std::cerr << "Error opening the SDC file." << std::endl;
        return 1;
    }
    unsigned max_msb = 0;
    unsigned rx_msb = 0;

    // Parse the JSON data from the file.
    nlohmann::json data;
    file >> data;
    /////////////////// check input connection
    // Iterate through the matches and append the SDC lines to the file
    for (const auto& [pin_loc, pinInfo] : data["locations"].items()) {
        std::string ball_id = pin_loc; // Assuming PIN_LOC corresponds to Ball ID
        std::string portName = pinInfo["name"];
        auto it = ballIdToCustomer.find(ball_id);
        std::string customerName = it->second;
        if (pinInfo.contains("index")) 
        {
            unsigned index = pinInfo["index"];
        }
        bool available = true;
        // Iterate over instConns
        for (const auto& entry : instConns) {
          const std::string& instance = entry.first;
          const std::vector<Connection>& instanceConnections = entry.second;
          for (const auto& conn : instanceConnections) {
                 std::vector<ioInfo> iosInfo = instIOs[instance];
                    for (const auto& io : iosInfo) {
                        if (io.ioDir == "OUT_DIR") {
                            if (io.ioName == "Q") {
                                rx_msb = std::max(rx_msb, io.msb);
                            }
                        } else if (io.ioDir == "IN_DIR") {
                            if (io.ioName == "D") {
                                max_msb = io.msb;
                            }
                        }
                    }
              if (conn.signal == portName) {
                if (std::find(used_pins.begin(), used_pins.end(), ball_id) != used_pins.end()) {
                    available = false;
                } else {
                    used_pins.push_back(ball_id);
                }
                if (conn.module.find("BUF") != std::string::npos) {
                    // If the pin is connected to BUF only
                     std::string mode = conn.module.find("I_") != std::string::npos ? "MODE_BP_DIR_A_TX" : "MODE_BP_DIR_A_RX";
                // Print other fields as needed
                  for (const auto& port : conn.ports) {
                      if ( available) {
                        if(port.first == "O") {
                          sdcFile << "set_property mode " << mode << " " << customerName << std::endl;
                          sdcFile << "set_pin_loc " << port.second << " " << customerName << " g2f_rx_in[0]" << std::endl;
                        } else if(port.first == "I") {
                          sdcFile << "set_property mode " << mode << " " << customerName << std::endl;
                          sdcFile << "set_pin_loc " << port.second << " " << customerName << " f2g_tx_out[0]" << std::endl;
                        }
                      } else {
                          throw std::runtime_error("Pin is already in use.");
                      }
                  }
                } else { // if SERDES or DDR is being used
                    for (const auto& port : conn.ports) {
                      if(conn.module.find("O_") != std::string::npos) {
                        if(port.first == "Q") {
                            // If port is Q, set constraints for input side which is connected to fabric
                          if (!instIOs[instance].empty()) {
                            std::vector<ioInfo> iosInfo = instIOs[instance];
                            for (const auto& io : iosInfo) {
                              if (io.ioDir == "IN_DIR"){
                                if(available) {
                                  unsigned j = (io.lsb > io.msb) ? io.lsb : io.msb;
                                  unsigned i = (io.lsb > io.msb)? io.msb : io.lsb;
                                  if(j!=0) {
                                    for (unsigned k = i; k <= j; k++) {
                                      std::string pin_name = io.actualName + "[" + std::to_string(k) + "]";
                                      if(io.ioName == "D") {
                                        sdcFile << "set_property mode " << "MODE_RATE_" << max_msb << "_A_TX "<< " " << customerName << std::endl;
                                        sdcFile << "set_pin_loc " << io.actualName << "[" << k << "] " << customerName << " f2g_tx_out[" << k << "]" << std::endl;
                                      } else {
                                        sdcFile << "set_property mode " <<"MODE_RATE_" << max_msb << "_A_TX " << " " << customerName << std::endl;
                                        sdcFile << "set_pin_loc " << io.actualName << "[" << k << "] " << customerName << " " << pinsMap[io.ioName] << "[" << k << "]" << std::endl;
                                      }
                                    }
                                  } else {
                                    sdcFile << "set_property mode " << "MODE_RATE_" << max_msb << "_A_TX "  << " " << customerName << std::endl;
                                    sdcFile << "set_pin_loc " << io.actualName << " " << customerName << " " << pinsMap[io.ioName] << std::endl;
                                  }
                                } else {
                                   throw std::runtime_error("Pin is already in use.");
                                }
                              }
                            }
                          }
                        }
                     } else if(conn.module.find("I_SERDES") != std::string::npos)
                       {
                        if(port.first == "D") {
                            // If port is D, set constraints for output side which is connected to fabric
                          if (!instIOs[instance].empty()) {
                            std::vector<ioInfo> iosInfo = instIOs[instance];
                            for (const auto& io : iosInfo) {
                                if (io.ioDir == "OUT_DIR"){
                                  if(available) {
                                  unsigned j = (io.lsb > io.msb) ? io.lsb : io.msb;
                                  unsigned i = (io.lsb > io.msb)? io.msb : io.lsb;
                                  if(j!=0) {
                                    for (unsigned k = i; k <= j; k++) {
                                      std::string pin_name = io.actualName + "[" + std::to_string(k) + "]";
                                      if(io.ioName == "Q") {
                                        sdcFile << "set_property mode " << "MODE_RATE_" << rx_msb << "_A_RX "<< " " << customerName << std::endl;
                                        sdcFile << "set_pin_loc " << io.actualName << "[" << k << "] " << customerName << " g2f_rx_in[" << k << "]" << std::endl;
                                      } else {
                                        sdcFile << "set_property mode " << "MODE_RATE_" << rx_msb << "_A_RX " << " " << customerName << std::endl;
                                        sdcFile << "set_pin_loc " << io.actualName << "[" << k << "] " << customerName << " " << pinsMap[io.ioName] << "[" << k << "]" << std::endl;
                                      }
                                    }
                                  } else {
                                    sdcFile << "set_property mode " << "MODE_RATE_" << rx_msb << "_A_RX " << " " << customerName << std::endl;
                                    sdcFile << "set_pin_loc " << io.actualName << " " << customerName << " " << pinsMap[io.ioName] << std::endl;
                                  }
                                } else {
                                   throw std::runtime_error("Pin is already in use.");
                                }
                              }
                            }
                          }
                        }
                    }
                  }
                }
              }
          }
      }
    }

       // Move to the next match
    

    // Close the SDC file
    sdcFile.close();
    file.close(); // Close the file

    return 0;
}

int update_sdc (std::string& intf_json, std::string& mod_ios, 
            std::string& map_json, std::string& pin_table, std::string& output_sdc)
{
	// Get original module's IOs
    get_io_info(mod_ios);
	// Get interface module's IOs
	getInstIos(intf_json);
    // For each original module IO
    for (const orig_io& entry : orig_ios)
	{
    	std::string io_name = entry.io_name;
    	unsigned lsb = entry.lsb;
    	unsigned msb = entry.msb;
    	unsigned dir = entry.dir;
		if (dir == VERI_INPUT) {
            // From original input, it gives the output of bufs (connected to fabric) if no SERDES or DDR are present
            // Otherwise it gives the corrsponding input of the SERDES or DDR (not connected to fabric)
    	    map_signal(intf_json, io_name, "Input");
    	} else if (dir == VERI_OUTPUT) {
            // From original output, it gives the input of bufs (connected to fabric) if no SERDES or DDR are present
            // Otherwise it gives the corrsponding output of the SERDES or DDR (not connected to fabric)
    	     map_signal(intf_json, io_name, "Output");
    	}
	}
	//printPinMap();
	write_sdc(map_json, pin_table, output_sdc);
    return 0;
}