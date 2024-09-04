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

#include "Utils.h"

namespace BITBLAST {

std::string Utils::escapeName(std::string_view name) {
  std::string result = std::string(name);
  result = "\\" + result + " ";
  return result;
}

std::string Utils::removeLibName(std::string_view name) {
  std::string result = std::string(name);
  result = result.substr(name.find("@") + 1);
  return result;
}

std::string Utils::printSpaces(uint32_t nb) {
  std::string result;
  for (uint32_t i = 0; i < nb; i++) {
    result += " ";
  }
  return result;
}

void Utils::tokenize(std::string_view str,
                                    std::string_view separator,
                                    std::vector<std::string> &result,
                                    bool skipEmpty) {
  std::string::size_type pos{0};
  const auto sepSize = separator.size();
  const auto stringSize = str.size();
  std::string tmp;
  std::string::size_type n = str.find(separator, pos);
  while (n != std::string::npos) {
    tmp = str.substr(pos, n - pos);
    if (!(tmp.empty() && skipEmpty)) result.push_back(tmp);
    pos = n + sepSize;
    n = str.find(separator, pos);
  }
  if (pos < stringSize) {  // put last part
    tmp = str.substr(pos, stringSize - pos);
    if (!(tmp.empty() && skipEmpty)) result.push_back(tmp);
  }
}

std::vector<std::string> Utils::tokenize(
    std::string_view str, std::string_view separator, bool skipEmpty) {
  std::vector<std::string> result;
  tokenize(str, separator, result, skipEmpty);
  return result;
}

}
