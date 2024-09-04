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

#if defined(_MSC_VER)
#include <direct.h>
#include <process.h>
#else
#include <sys/param.h>
#include <unistd.h>
#endif

#include <string.h>
#include <sys/stat.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#if defined(_MSC_VER) && defined(_DEBUG)
#include <Windows.h>
#endif

#include <Surelog/API/Surelog.h>
#include <Surelog/CommandLine/CommandLineParser.h>
#include <Surelog/ErrorReporting/ErrorContainer.h>
#include <Surelog/SourceCompile/SymbolTable.h>
#include <uhdm/uhdm.h>

#include "NetlistPrettyPrinter.h"

int main(int argc, const char **argv) {
  if (argc < 2) return 0;

  std::string output_file;
  // Read Surelog command line 
  // -bitblast <file> : Outputs the bitblasted netlist into file <file>
  int32_t code = 0;
  SURELOG::SymbolTable *const symbolTable = new SURELOG::SymbolTable();
  for (int i = 0; i < argc; i++) {
    std::string arg = argv[i];
    if (arg == "-bitblast") {
      output_file = argv[i+1];
      argv[i] = strdup("");
      argv[i+1] = strdup("");
    }
  }

  SURELOG::ErrorContainer *const errors =
      new SURELOG::ErrorContainer(symbolTable);
  SURELOG::CommandLineParser *const clp =
      new SURELOG::CommandLineParser(errors, symbolTable, false, false);
  clp->noPython();
  clp->setwritePpOutput(true);
  clp->setParse(true);
  clp->setCompile(true);
  clp->setElabUhdm(false);  // Request (Not) UHDM Uniquification/Elaboration
  clp->setElaborate(true);  // Request Surelog instance tree elaboration
  bool success = clp->parseCommandLine(argc, argv);
  errors->printMessages(clp->muteStdout());

  vpiHandle vpi_design = nullptr;
  SURELOG::scompiler *compiler = nullptr;
  if (success && (!clp->help())) {
    compiler = SURELOG::start_compiler(clp);
    vpi_design = SURELOG::get_uhdm_design(compiler);
    auto stats = errors->getErrorStats();
    code = (!success) | stats.nbFatal | stats.nbSyntax | stats.nbError;
  }

  SURELOG::ErrorContainer::Stats stats = errors->getErrorStats();
  errors->printStats(stats, false);

  if (vpi_design == nullptr) return code;

  if (!output_file.empty())
  {
    BITBLAST::NetlistPrettyPrinter *printer =
        new BITBLAST::NetlistPrettyPrinter();
    std::string result =
        printer->prettyPrint(UhdmDesignFromVpiHandle(vpi_design));
    delete printer;

    std::cout << "DESIGN:\n"
              << result << "\n";

    std::ofstream ofs(output_file);
    if (ofs.good())
    {
      ofs << result;
      ofs << std::flush;
      ofs.close();
    }
  }

  // Do not delete these objects until you are done with UHDM
  SURELOG::shutdown_compiler(compiler);
  delete clp;
  delete symbolTable;
  delete errors;

  return 0;
}
