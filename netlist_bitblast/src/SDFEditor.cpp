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
#include <iostream>
#include <sstream>
#include <fstream>
#include "Utils.h"

namespace BITBLAST {

bool SDFEditor::edit(BitBlaster *blaster, std::filesystem::path sdfInputFile,
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
  for (uint32_t i = 0; i < lines.size(); i++) {
    std::string_view line = lines[i];
    std::string tmp = Utils::replaceAll(line, "in[0]", "in0");
    tmp = Utils::replaceAll(tmp, "in[1]", "in1");
    tmp = Utils::replaceAll(tmp, "in[2]", "in2");
    tmp = Utils::replaceAll(tmp, "in[3]", "in3");
    tmp = Utils::replaceAll(tmp, "in[4]", "in4");
    tmp = Utils::replaceAll(tmp, "in[5]", "in5");
    tmp = Utils::replaceAll(tmp, "\"dffre\"", "\"DFFRE\"");
    tmp = Utils::replaceAll(tmp, "\"dffnre\"", "\"DFFNRE\"");
    if (tmp.find("(CELLTYPE ") != std::string::npos) {
      std::string line_plus1 = std::string(lines[i+1]);
      auto itr = line_plus1.find("(INSTANCE ");
      std::string instance = line_plus1.substr(itr + 10);
      instance = instance.substr(0, instance.size()-2);
      instance = Utils::replaceAll(instance, "\\", "");
      std::string cellType = blaster->getCellType(instance);
      //std::cout << "INSTANCE: " << instance << " CELL: " << cellType << std::endl;
      if (!cellType.empty()) {
        tmp = Utils::replaceAll(tmp, "LUT_K", cellType);
      }
    }
    result += tmp;
  }

  std::ofstream output(sdfOutputFile.string());
  if (!output.good()) {
    std::cerr << "Can't create SDF file: " << sdfOutputFile.string() << std::endl
              << std::flush;
    exit(1);
  }

  output << result;
  output.close();

  return true;
}

}  // namespace BITBLAST
