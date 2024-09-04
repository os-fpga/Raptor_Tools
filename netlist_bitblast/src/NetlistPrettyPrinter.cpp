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

#include "NetlistPrettyPrinter.h"

#include <uhdm/ElaboratorListener.h>
#include <uhdm/ExprEval.h>
#include <uhdm/NumUtils.h>
#include <uhdm/clone_tree.h>
#include <uhdm/containers.h>
#include <uhdm/uhdm.h>
#include <uhdm/uhdm_forward_decl.h>
#include <uhdm/uhdm_types.h>

using namespace UHDM;

namespace BITBLAST {

std::string NetlistPrettyPrinter::escapeName(std::string_view name) {
  std::string result = std::string(name);
  result = "\\" + result + " ";
  return result;
}

std::string NetlistPrettyPrinter::removeLibName(std::string_view name) {
  std::string result = std::string(name);
  result = result.substr(name.find("@") + 1);
  return result;
}

std::string NetlistPrettyPrinter::printSpaces(uint32_t nb) {
  std::string result;
  for (uint32_t i = 0; i < nb; i++) {
    result += " ";
  }
  return result;
}

void NetlistPrettyPrinter::tokenize(std::string_view str,
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

std::vector<std::string> NetlistPrettyPrinter::tokenize(
    std::string_view str, std::string_view separator, bool skipEmpty) {
  std::vector<std::string> result;
  tokenize(str, separator, result, skipEmpty);
  return result;
}

std::string NetlistPrettyPrinter::prettyPrint(const UHDM::any *handle) {
  if (handle == nullptr) {
    std::cout << "NULL HANDLE\n";
    return "NULL HANDLE";
  }
  Serializer *s = handle->GetSerializer();
  std::stringstream out;
  prettyPrint(*s, handle, 0, out);
  return out.str();
}

void NetlistPrettyPrinter::prettyPrint(UHDM::Serializer &s,
                                       const UHDM::any *object, uint32_t indent,
                                       std::ostream &out) {
  if (object == nullptr) return;
  UHDM::UHDM_OBJECT_TYPE type = object->UhdmType();
  for (uint32_t i = 0; i < indent; i++) {
    out << " ";
  }
  switch (type) {
    case UHDM_OBJECT_TYPE::uhdmdesign: {
      design *d = (design *)object;
      m_design = d;
      for (module_inst *top : *d->TopModules()) {
        prettyPrint(s, top, 0, out);
      }
      break;
    }
    case UHDM_OBJECT_TYPE::uhdmmodule_inst: {
      module_inst *c = (module_inst *)object;
      if (c->VpiTopModule()) {
        out << "`timescale 1ns/1ps\n";
        out << "module ";
        std::string name = removeLibName(c->VpiName());
        out << name;
        out << " (\n";
        int nbPorts = c->Ports()->size();
        int index = 0;
        for (port *p : *c->Ports()) {
          prettyPrint(s, p, 4, out);
          index++;
          if (index < nbPorts) out << ",\n";
        }
        out << "\n);\n";
        out << "\n";
        out << "    //Wires\n";
        for (net *p : *c->Nets()) {
          prettyPrint(s, p, 4, out);
        }
        out << "\n";
        out << "    //IO assignments\n";
        for (cont_assign *as : *c->Cont_assigns()) {
          prettyPrint(s, as, 4, out);
        }
        out << "\n";
        for (module_inst *m : *c->Modules()) {
          prettyPrint(s, m, 4, out);
        }
        out << "\n";
        out << "endmodule";
        out << "\n";
      } else {
        std::string cellName = removeLibName(c->VpiDefName());
        if (cellName == "fpga_interconnect") {
          if (!m_printed_interconnect) {
            out << "//Interconnect\n";
            out << printSpaces(indent);
            m_printed_interconnect = true;
          }
        }
        if (cellName != "fpga_interconnect") {
          if (!m_printed_cell_instances) {
            out << "\n" << printSpaces(indent);
            out << "//Cell instances\n";
            out << printSpaces(indent);
            m_printed_cell_instances = true;
          }
        }
        out << cellName;
        if (cellName != "fpga_interconnect") {
          out << " #(\n";
        }
        if (c->Param_assigns()) {
          int nbParams = c->Param_assigns()->size();
          int index = 0;
          for (param_assign *p : *c->Param_assigns()) {
            prettyPrint(s, p, 8, out);
            index++;
            if (index < nbParams) out << ",";
            out << "\n";
          }
        }
        if (cellName != "fpga_interconnect") {
          out << "    )";
        }
        out << " ";
        out << escapeName(c->VpiName());
        out << " ";
        out << "(\n";
        if (c->Ports()) {
          int nbPorts = c->Ports()->size();
          int index = 0;
          for (port *p : *c->Ports()) {
            out << "        ." << p->VpiName() << "(";
            ExprEval eval;
            std::stringstream outtmp;
            eval.prettyPrint(s, p->High_conn(), 0, outtmp);
            std::string tmps = outtmp.str();
            if (tmps.find("{") == std::string::npos) {
              tmps = escapeName(tmps);
              out << tmps;
            } else {
              tmps.erase(0, 1);
              tmps.erase(tmps.size() - 1);
              out << "{\n";
              std::vector<std::string> tokens = tokenize(tmps, ",");
              int nbTokens = tokens.size();
              int index = 0;
              for (auto token : tokens) {
                out << printSpaces(12);
                if (token[0] == '1')
                  out << token;
                else
                  out << escapeName(token);
                index++;
                if (index < nbTokens) out << ",";
                out << "\n";
              }
              out << printSpaces(8) << " }";
            }
            out << ")";
            index++;
            if (index < nbPorts) out << ",";
            out << "\n";
          }
        }
        out << "    )";
        out << ";";
        out << "\n\n";
      }
      break;
    }
    case UHDM_OBJECT_TYPE::uhdmport: {
      port *p = (port *)object;
      uint32_t type = p->VpiDirection();
      if (type == vpiInput) {
        out << "input ";
      } else if (type == vpiOutput) {
        out << "output ";
      } else if (type == vpiInout) {
        out << "inout ";
      } else {
        out << "unknown:" << type;
      }
      out << escapeName(p->VpiName());
      break;
    }
    case UHDM_OBJECT_TYPE::uhdmlogic_net: {
      logic_net *n = (logic_net *)object;
      uint32_t type = n->VpiNetType();
      if (type == 0) {
        // Implicit type
        out << "wire ";
      } else if (type == vpiWire) {
        out << "wire ";
      } else if (type == vpiReg) {
        out << "reg ";
      } else {
        std::cerr << "NOT HANDLED NET TYPE: " << type << "\n";
        exit(1);
      }
      out << escapeName(n->VpiName());
      out << ";\n";
      break;
    }
    case UHDM_OBJECT_TYPE::uhdmparam_assign: {
      param_assign *p = (param_assign *)object;
      any *lhs = p->Lhs();
      any *rhs = p->Rhs();
      out << "." << lhs->VpiName() << "(";
      ExprEval eval;
      eval.prettyPrint(s, rhs, 0, out);
      out << ")";
      break;
    }
    case UHDM_OBJECT_TYPE::uhdmcont_assign: {
      cont_assign *cont = (cont_assign *)object;
      out << "assign ";
      ExprEval eval;
      std::stringstream outtmp;
      eval.prettyPrint(s, cont->Lhs(), 0, outtmp);
      std::string tmps = outtmp.str();
      tmps = escapeName(tmps);
      out << tmps;
      out << " = ";
      std::stringstream outtmp1;
      eval.prettyPrint(s, cont->Rhs(), 0, outtmp1);
      tmps = outtmp1.str();
      tmps = escapeName(tmps);
      out << tmps;
      out << ";\n";
      break;
    }
    default: {
      std::cerr << "NOT HANDLED HANDLE: " << UhdmName(object->UhdmType())
                << "\n";
      exit(1);
      break;
    }
  }
}

}  // namespace BITBLAST
