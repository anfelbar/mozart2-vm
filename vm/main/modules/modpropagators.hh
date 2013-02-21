#ifndef __MODINTVARPROP_H
#define __MODINTVARPROP_H

#include "../mozartcore.hh"
#include <gecode/int.hh>

namespace mozart {

namespace builtins {

// Notes:
// - All the constraints are posted in vm->getCurrentSpace()
//   TODO: can I assume that the current space is the same space each
//   variable belongs to?
class ModIntVarProp: public Module {
public:
  ModIntVarProp(): Module("IntVarProp") {}

  static Gecode::IntRelType atomToRelType(VM vm, In r) {
    atom_t a = getArgument<atom_t>(vm,r,MOZART_STR("Atom"));
    if (a == vm->coreatoms.irt_eq) return Gecode::IRT_EQ;
    if (a == vm->coreatoms.irt_nq) return Gecode::IRT_NQ;
    if (a == vm->coreatoms.irt_lq) return Gecode::IRT_LQ;
    if (a == vm->coreatoms.irt_le) return Gecode::IRT_LE;
    if (a == vm->coreatoms.irt_gq) return Gecode::IRT_GQ;
    if (a == vm->coreatoms.irt_gr) return Gecode::IRT_GR;
    raiseTypeError(vm,MOZART_STR("InvalidRelationType"),r);
    return Gecode::IRT_GR;
  }

  static Gecode::IntConLevel atomToIntConLevel(VM vm, In l) {
    atom_t a = getArgument<atom_t>(vm,l,MOZART_STR("Atom"));
    if (a == vm->coreatoms.icl_val) return Gecode::ICL_VAL;
    if (a == vm->coreatoms.icl_bnd) return Gecode::ICL_BND;
    if (a == vm->coreatoms.icl_dom) return Gecode::ICL_DOM;
    if (a == vm->coreatoms.icl_def) return Gecode::ICL_DEF;
    raiseTypeError(vm,MOZART_STR("InvalidIntegerConsistencyLevel"),l);
    return Gecode::ICL_DEF;
  }

  static Gecode::IntVarArgs getIntVarArgs(VM vm, In x){
    size_t width;
    
    if(x.is<Tuple>()){
      width = x.as<Tuple>().getWidth();
      Gecode::IntVarArgs v(width);
      std::cout << "Is tuple" << width << std::endl;
      for(int i=0; i<width; i++){
      	StableNode* t=x.as<Tuple>().getElement(i);
	UnstableNode a = Reference::build(vm, t);
	RichNode tt = a;
	assert(tt.is<CstIntVar>());
	v[i] = IntVarLike(tt.as<CstIntVar>()).intVar(vm);
      }
      for(int i=0; i<width; i++){
	std::cout << v[i].min() << std::endl;
      }
      return v;
    }else if(x.is<Cons>()){
      std::cout << "Args is Cons" << std::endl;
      Gecode::IntVarArgs v(1);
      return v;
    }else {
      std::cout << "Args is Record" << std::endl;
      Gecode::IntVarArgs v(1);
      return v;
    }
  }
  
  class Rel: public Builtin<Rel> {
  public:
    Rel(): Builtin("rel") {}

    static void call(VM vm, In x0, In r, In x1, In icl) {
      assert(vm->getCurrentSpace()->hasConstraintSpace());
      GecodeSpace& home = vm->getCurrentSpace()->getCstSpace();
      Gecode::IntRelType rt = atomToRelType(vm,r);
      Gecode::rel(home,IntVarLike(x0).intVar(vm),rt,IntVarLike(x1).intVar(vm));
    }
  };

  class Distinct: public Builtin<Distinct> {
  public:
    Distinct(): Builtin("distinct") {}
    
    static void call(VM vm, In x) {
      assert(vm->getCurrentSpace()->hasConstraintSpace());
      GecodeSpace& home = vm->getCurrentSpace()->getCstSpace();
      Gecode::distinct(home,getIntVarArgs(vm,x));
      //home.status(); //Call status in order to propagate; Debuging porpuses. 
    }
  };

}; // class ModIntVarProp
} // namespace builtins
} // namespace mozart
#endif // __MODINTVARPROP_H
