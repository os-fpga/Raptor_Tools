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
#include "BitBlaster.h"

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

bool BitBlaster::bitBlast(const UHDM::any *object) {
  if (object == nullptr) return false;
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

}  // namespace BITBLAST
