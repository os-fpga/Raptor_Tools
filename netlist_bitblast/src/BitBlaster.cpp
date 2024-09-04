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
#include "BitBlaster.h"

#include <uhdm/ElaboratorListener.h>
#include <uhdm/ExprEval.h>
#include <uhdm/NumUtils.h>
#include <uhdm/clone_tree.h>
#include <uhdm/containers.h>
#include <uhdm/uhdm.h>
#include <uhdm/uhdm_forward_decl.h>
#include <uhdm/uhdm_types.h>

#include "Utils.h"

using namespace UHDM;

namespace BITBLAST {

bool BitBlaster::bitBlast(const UHDM::any *object) {
  if (object == nullptr) return false;
  Serializer *s = object->GetSerializer();
  UHDM::UHDM_OBJECT_TYPE type = object->UhdmType();
  switch (type) {
    case UHDM_OBJECT_TYPE::uhdmdesign: {
      design *d = (design *)object;
      m_design = d;
      for (module_inst *top : *d->TopModules()) {
        bitBlast(top);
      }
      break;
    }
    case UHDM_OBJECT_TYPE::uhdmmodule_inst: {
      module_inst *c = (module_inst *)object;
      if (c->VpiTopModule()) {
        if (c->Modules()) {
          for (module_inst *m : *c->Modules()) {
            bitBlast(m);
          }
        }
      } else {
        std::string cellName = Utils::removeLibName(c->VpiDefName());
        if (cellName == "LUT_K") {
          uint64_t k = 0;
          if (c->Param_assigns()) {
            for (param_assign *p : *c->Param_assigns()) {
              any *lhs = p->Lhs();
              any *rhs = p->Rhs();
              if (lhs->VpiName() == "K") {
                ExprEval eval;
                k = eval.getValue((expr *)rhs);
              }
            }
          }
          std::string blastedName = "LUT_K" + std::to_string(k);
          m_instanceCellMap.emplace(std::string(c->VpiName()), blastedName);
          c->VpiDefName(blastedName);
          if (auto origPorts = c->Ports()) {
            VectorOfport *newPorts = s->MakePortVec();
            for (port *p : *origPorts) {
              any *lowc = p->Low_conn();
              if (lowc->VpiName() == "in") {
                any *highc = p->High_conn();
                UHDM_OBJECT_TYPE high_conn_type = highc->UhdmType();
                if (high_conn_type == uhdmconstant) {
                  constant *c = (constant *)highc;
                  ExprEval eval;
                  uint64_t val = eval.getValue(c);
                  for (uint64_t i = 0; i < k; i++) {
                    port *np = s->MakePort();
                    np->VpiName("in" + std::to_string(i));
                    constant *cn = s->MakeConstant();
                    cn->VpiSize(1);
                    cn->VpiConstType(vpiBinaryConst);
                    cn->VpiValue("BIN:" + std::to_string(val &= i));
                    np->High_conn(cn);
                    newPorts->push_back(np);
                  }
                } else if (high_conn_type == uhdmoperation) {
                  operation *oper = (operation *)highc;
                  int index = 0;
                  for (any *op : *oper->Operands()) {
                    port *np = s->MakePort();
                    np->VpiName("in" + std::to_string(index));
                    np->High_conn(op);
                    newPorts->push_back(np);
                    index++;
                  }
                }
              } else {
                newPorts->push_back(p);
              }
            }
            c->Ports(newPorts);
          }
        }
      }
      break;
    }
    default: {
      std::cerr << "NOT HANDLED HANDLE: " << UhdmName(object->UhdmType())
                << "\n";
      exit(1);
      break;
    }
  }
  return true;
}

static std::string empty;
const std::string& BitBlaster::getCellType(const std::string& instance) {
   std::map<std::string, std::string>::iterator itr = m_instanceCellMap.find(instance);
  if (itr == m_instanceCellMap.end()) {
    return empty;
  } else {
    return (*itr).second;
  }
}

}  // namespace BITBLAST
