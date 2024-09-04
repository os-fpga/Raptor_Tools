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
#ifndef SDF_EDITOR_H
#define SDF_EDITOR_H

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || \
    defined(_MSC_VER) || defined(__CYGWIN__)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#ifndef __GNUC__
#include <direct.h>
#endif
#include <process.h>
#ifndef __SIZEOF_INT__
#define __SIZEOF_INT__ sizeof(int)
#endif
#else
#include <stdlib.h>
#include <sys/param.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#include <uhdm/uhdm.h>

#include <filesystem>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "BitBlaster.h"
namespace BITBLAST {

class SDFEditor {
 public:
  bool edit(BitBlaster* blaster, std::filesystem::path sdfInputFile,
            std::filesystem::path sdfOutputFile);

 protected:
 private:
};

}  // namespace BITBLAST

#endif
