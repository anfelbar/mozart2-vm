#ifndef __CSTBOOLVAR_H
#define __CSTBOOLVAR_H

#include "mozartcore.hh"

#ifndef MOZART_GENERATOR

namespace mozart {
#include "CstBoolVar-implem.hh"

CstBoolVar::CstBoolVar(VM vm) 
  : WithHome(vm), _varIndex(0) {
  auto& sp = home()->getCstSpace();
  _varIndex = sp.newBoolVar();
}


} // mozart

#endif // MOZART_GENERATOR

#endif // __CSTBOOLVAR_H
