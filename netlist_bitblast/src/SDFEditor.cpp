/*
Copyright 2021-2024 Rapid Silicon

GPL License

Copyright (c) 2024 The Open-Source FPGA Foundation

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "SDFEditor.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>

#include "Utils.h"

namespace BITBLAST {

std::map<std::string, std::vector<std::string>> cellTypePortsToBlastMap = {
    {"DEFAULT", {}},
    {"LUT_K", {"in"}},
    {"RS_TDP", {"RDATA"}},
    {"RS_DSP", {"a", "b", "z", "dly_b", "feedback", "acc_fir", "shift_right"}}};

bool SDFEditor::edit(BitBlaster* blaster, std::filesystem::path sdfInputFile,
                     std::filesystem::path sdfOutputFile) {
  std::ifstream stream(sdfInputFile.string());
  if (!stream.good()) {
    std::cerr << "Can't read SDF file: " << sdfInputFile.string() << std::endl
              << std::flush;
    exit(1);
  }
  std::string sdf_in((std::istreambuf_iterator<char>(stream)),
                     std::istreambuf_iterator<char>());
  stream.close();

  std::vector<std::string_view> lines = Utils::splitLines(sdf_in);

  std::string result;
  std::vector<std::string>& portsToBlast = cellTypePortsToBlastMap["DEFAULT"];
  std::string baseCellName;
  for (uint32_t i = 0; i < lines.size(); i++) {
    std::string_view line = lines[i];
    std::string tmp = std::string(line);
    // Change module type to the corresponding basted type
    if (tmp.find("(CELLTYPE ") != std::string::npos) {
      static std::regex expr(R"(\"([a-zA-Z0-9_]+)\")");
      std::smatch match;
      if (std::regex_search(tmp, match, expr)) {
        std::string origCellType = match[1].str();
        if (origCellType.find("LUT_K") != std::string::npos) {
          baseCellName = "LUT_K";
        } else if (origCellType.find("RS_TDP") != std::string::npos) {
          baseCellName = "RS_TDP";
        } else if (origCellType.find("RS_DSP") != std::string::npos) {
          baseCellName = "RS_DSP";
        }
        std::map<std::string, std::vector<std::string>>::const_iterator
            itrPort = cellTypePortsToBlastMap.find(baseCellName);
        if (itrPort != cellTypePortsToBlastMap.end()) {
          portsToBlast = (*itrPort).second;
        }

        std::string line_plus1 = std::string(lines[i + 1]);
        auto itr = line_plus1.find("(INSTANCE ");
        std::string instance = line_plus1.substr(itr + 10);
        instance = instance.substr(0, instance.size() - 2);
        instance = Utils::replaceAll(instance, "\\", "");
        std::string cellType = blaster->getCellType(instance);
        // std::cout << "ORIG: " << origCellType << " INSTANCE: " << instance <<
        // " CELL: " << cellType << std::endl;
        if (!cellType.empty()) {
          tmp = Utils::replaceAll(tmp, origCellType, cellType);
        }
      }
    }

    // LUTs and DSPs ports, no-op for BRAMs
    if (baseCellName == "LUT_K" || baseCellName == "RS_DSP") {
      for (std::string port : portsToBlast) {
        std::regex expr(port + "\\[([0-9]+)\\]");
        std::smatch match;
        if (std::regex_search(tmp, match, expr)) {
          std::string index = match[1].str();
          tmp = Utils::replaceAll(tmp, port + "[" + index + "]", port + index);
        }
      }
    }

    if (baseCellName == "RS_TDP") {
      // Special BRAMs ports duplication
      static std::regex exprdata(R"(RDATA_([a-zA-Z0-9_]+))");
      std::smatch matchdata;
      if (std::regex_search(tmp, matchdata, exprdata)) {
        std::string origPort = std::string("RDATA_") + matchdata[1].str();
        std::string orig = tmp;
        tmp = Utils::replaceAll(tmp, origPort, origPort + "_0");
        for (int i = 1; i <= /*TODO: find from model the port size: */ 17;
             i++) {
          tmp += Utils::replaceAll(orig, origPort,
                                   origPort + "_" + std::to_string(i));
        }
      }
    }
    if (baseCellName == "RS_DSP") {
      // Special DSPs ports duplication
      if (tmp.find("dly_b ") != std::string::npos) {
        std::string origPort = "dly_b";
        std::string orig = tmp;
        tmp = Utils::replaceAll(tmp, origPort, origPort + "0");
        for (int i = 1; i <= /*TODO: find from model the port size: */ 17;
             i++) {
          tmp +=
              Utils::replaceAll(orig, origPort, origPort + std::to_string(i));
        }
      }

      if (tmp.find("z ") != std::string::npos) {
        std::string origPort = "z";
        std::string orig = tmp;
        tmp = Utils::replaceAll(tmp, origPort, origPort + "0");
        for (int i = 1; i <= /*TODO: find from model the port size: */ 37;
             i++) {
          tmp +=
              Utils::replaceAll(orig, origPort, origPort + std::to_string(i));
        }
      }
    }

    // Reverse map to simulation model names for dffs
    tmp = Utils::replaceAll(tmp, "\"dffre\"", "\"DFFRE\"");
    tmp = Utils::replaceAll(tmp, "\"dffnre\"", "\"DFFNRE\"");

    result += tmp;
  }

  std::ofstream output(sdfOutputFile.string());
  if (!output.good()) {
    std::cerr << "Can't create SDF file: " << sdfOutputFile.string()
              << std::endl
              << std::flush;
    exit(1);
  }

  output << result;
  output.close();

  return true;
}

}  // namespace BITBLAST
