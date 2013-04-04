// Copyright © 2011, Université catholique de Louvain
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// *  Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// *  Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef __MODINTVAR_H
#define __MODINTVAR_H

#include "../mozartcore.hh"

#ifndef MOZART_GENERATOR

namespace mozart {

namespace builtins {

///////////////////
// IntVar module //
///////////////////

class ModIntVar: public Module {
public:
  ModIntVar(): Module("IntVar") {}

  class New: public Builtin<New> {
  public:
    New(): Builtin("new") {}

    static void call(VM vm, In min, In max, Out result) {
      result = CstIntVar::build(vm,min,max);
    }
  };

  class Is: public Builtin<Is> {
  public:
    Is(): Builtin("is") {}

    static void call(VM vm, In var, Out result) {
      result = build(vm, IntVarLike(var).isIntVarLike(vm));
    }
  };

  class Min: public Builtin<Min> {
  public:
    Min(): Builtin("min") {}

    static void call(VM vm, In var, Out result) {
      result = build(vm, IntVarLike(var).min(vm));
    }
  };

  class Max: public Builtin<Max> {
  public:
    Max(): Builtin("max") {}

    static void call(VM vm, In var, Out result) {
      result = build(vm, IntVarLike(var).max(vm));
    }
  };

  class Value: public Builtin<Value> {
  public:
    Value(): Builtin("value") {}

    static void call(VM vm, In var, Out result) {
      result = build(vm, IntVarLike(var).value(vm));
    }
  };

  class IsIn: public Builtin<IsIn> {
  public:
    IsIn(): Builtin("isIn") {}

    static void call(VM vm, In var, In n, Out result) {
      result = build(vm, IntVarLike(var).isIn(vm,n));
    }
  };
  class Inf: public Builtin<Inf> {
  public:
    Inf(): Builtin("inf") {}

    static void call(VM vm, Out result) {
      result = build(vm, Gecode::Int::Limits::min);
    }
  };

  class Sup: public Builtin<Sup> {
  public:
    Sup(): Builtin("sup") {}

    static void call(VM vm, Out result) {
      result = build(vm, Gecode::Int::Limits::max);
    }
  };

};
} // builtins
} // mozart

#endif // MOZART_GENERATOR
#endif // __MODINTVAR_H
