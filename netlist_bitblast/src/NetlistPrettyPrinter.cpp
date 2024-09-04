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
  if (result.find("$") != std::string::npos) {
    result = "\\" + result + " ";
  }

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
      out << "`timescale 1ns/1ps\n";
      out << "module ";
      std::string name = std::string(c->VpiName());
      name = name.substr(name.find("@") + 1);
      out << name;
      out << "(";
      int nbPorts = c->Ports()->size();
      int index = 0;
      for (port *p : *c->Ports()) {
        prettyPrint(s, p, 0, out);
        index++;
        if (index < nbPorts) out << ", ";
      }
      out << ");\n";
      for (net *p : *c->Nets()) {
        prettyPrint(s, p, 0, out);
      }
      out << "\n";
      out << "endmodule";
      out << "\n";
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
        std::cout << "NOT HANDLED NET TYPE: " << type << "\n";
      }
      out << escapeName(n->VpiName());
      out << ";\n";
      break;
    }
    default: {
      std::cout << "NOT HANDLED HANDLE: " << UhdmName(object->UhdmType())
                << "\n";
      break;
    }
  }
}

}  // namespace BITBLAST
