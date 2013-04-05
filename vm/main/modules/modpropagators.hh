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
  
  static Gecode::IntVarBranch atomToIntVarBranch(VM vm, In bvar){
    atom_t a = getArgument<atom_t>(vm,bvar,MOZART_STR("Atom"));
    if (a == vm->coreatoms.int_var_none) return Gecode::INT_VAR_NONE;
    if (a == vm->coreatoms.int_var_rnd) return Gecode::INT_VAR_RND;
    if (a == vm->coreatoms.int_var_degree_min) return Gecode::INT_VAR_DEGREE_MIN;
    if (a == vm->coreatoms.int_var_degree_max) return Gecode::INT_VAR_DEGREE_MAX;
    if (a == vm->coreatoms.int_var_afc_min) return Gecode::INT_VAR_AFC_MIN;
    if (a == vm->coreatoms.int_var_min_min) return Gecode::INT_VAR_MIN_MIN;
    if (a == vm->coreatoms.int_var_min_max) return Gecode::INT_VAR_MIN_MAX;
    if (a == vm->coreatoms.int_var_max_min) return Gecode::INT_VAR_MAX_MIN;
    if (a == vm->coreatoms.int_var_max_max) return Gecode::INT_VAR_MAX_MAX;
    if (a == vm->coreatoms.int_var_size_min) return Gecode::INT_VAR_SIZE_MIN;
    if (a == vm->coreatoms.int_var_size_max) return Gecode::INT_VAR_SIZE_MAX;
    if (a == vm->coreatoms.int_var_size_degree_min) return Gecode::INT_VAR_SIZE_DEGREE_MIN;
    if (a == vm->coreatoms.int_var_size_degree_max) return Gecode::INT_VAR_SIZE_DEGREE_MAX;
    if (a == vm->coreatoms.int_var_size_afc_min) return Gecode::INT_VAR_SIZE_AFC_MIN;
    if (a == vm->coreatoms.int_var_size_afc_max) return Gecode::INT_VAR_SIZE_AFC_MAX;
    if (a == vm->coreatoms.int_var_regret_min_min) return Gecode::INT_VAR_REGRET_MIN_MIN;
    if (a == vm->coreatoms.int_var_regret_min_max) return Gecode::INT_VAR_REGRET_MIN_MAX;
    if (a == vm->coreatoms.int_var_regret_max_min) return Gecode::INT_VAR_REGRET_MAX_MIN;
    if (a == vm->coreatoms.int_var_regret_max_max) return Gecode::INT_VAR_REGRET_MAX_MAX;
    raiseTypeError(vm,MOZART_STR("InvalidIntegerBranchType"),bvar);
    return Gecode::INT_VAR_NONE;
  }

  static Gecode::IntValBranch atomToIntValBranch(VM vm, In bval){
    atom_t a = getArgument<atom_t>(vm,bval,MOZART_STR("Atom"));  
    if (a == vm->coreatoms.int_val_min) return Gecode::INT_VAL_MIN;
    if (a == vm->coreatoms.int_val_med) return Gecode::INT_VAL_MED;
    if (a == vm->coreatoms.int_val_max) return Gecode::INT_VAL_MAX;
    if (a == vm->coreatoms.int_val_rad) return Gecode::INT_VAL_RND;
    if (a == vm->coreatoms.int_val_split_min) return Gecode::INT_VAL_SPLIT_MIN;
    if (a == vm->coreatoms.int_val_split_max) return Gecode::INT_VAL_SPLIT_MAX;
    if (a == vm->coreatoms.int_val_range_min) return Gecode::INT_VAL_RANGE_MIN;
    if (a == vm->coreatoms.int_val_range_max) return Gecode::INT_VAL_RANGE_MAX;
    if (a == vm->coreatoms.int_values_min) return Gecode::INT_VALUES_MIN;
    if (a == vm->coreatoms.int_values_max) return Gecode::INT_VALUES_MAX;
    raiseTypeError(vm,MOZART_STR("InvalidIntegerBranchType"),bval);
    return Gecode::INT_VAL_MIN;
  }

  static Gecode::IntVarArgs getIntVarArgs(VM vm, In x){
    size_t width;
    
    if(x.is<Tuple>()){
      width = x.as<Tuple>().getWidth();
      Gecode::IntVarArgs v(width);
      for(unsigned int i=0; i<width; i++){
      	StableNode* t=x.as<Tuple>().getElement(i);
	UnstableNode a = Reference::build(vm, t);
	RichNode tt = a;
	assert(tt.is<CstIntVar>());
	v[i] = IntVarLike(tt.as<CstIntVar>()).intVar(vm);
      }
      return v;
    }else if(x.is<Cons>()){
      Gecode::IntVarArgs v;
      StableNode* head=x.as<Cons>().getHead();
      StableNode* tail=x.as<Cons>().getTail();
      while (true){
      	UnstableNode uhead = Reference::build(vm, head);
      	RichNode rhead = uhead;
      	assert(rhead.is<CstIntVar>());
      	v << IntVarLike(rhead.as<CstIntVar>()).intVar(vm);
	UnstableNode utail = Reference::build(vm, tail);
      	RichNode rtail = utail;
	if (!rtail.is<Cons>()){
	  break;
	}
      	UnstableNode ncons = Reference::build(vm, tail);
      	RichNode rncons = ncons;
      	head=rncons.as<Cons>().getHead();
	tail=rncons.as<Cons>().getTail();	
      }
      return v;
    }else {
      std::cout << "Args is Record" << std::endl;
      width = x.as<Record>().getWidth();
      Gecode::IntVarArgs v(width);
      for(unsigned int i=0; i<width; i++){
      	StableNode* t=x.as<Record>().getElement(i);
      	UnstableNode a = Reference::build(vm, t);
      	RichNode tt = a;
      	assert(tt.is<CstIntVar>());
      	v[i] = IntVarLike(tt.as<CstIntVar>()).intVar(vm);
      }
      return v;
    }
  }
  
  static Gecode::IntArgs getIntArgs(VM vm, In x){
    std::vector<int> v;
    StableNode* head=x.as<Cons>().getHead();
    StableNode* tail=x.as<Cons>().getTail();
    while (true){
      UnstableNode uhead = Reference::build(vm, head);
      RichNode rhead = uhead;
      assert(rhead.is<SmallInt>());
      nativeint val=rhead.as<SmallInt>().value();
      v.push_back((int)(val)); 
      UnstableNode utail = Reference::build(vm, tail);
      RichNode rtail = utail;
      if (!rtail.is<Cons>()){
	break;
      }
      UnstableNode ncons = Reference::build(vm, tail);
      RichNode rncons = ncons;
      head=rncons.as<Cons>().getHead();
      tail=rncons.as<Cons>().getTail();
	
    }
    return Gecode::IntArgs(v);
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
    }
  };

  class Linear: public Builtin<Linear> {
  public:
    Linear(): Builtin("linear") {}
    
    static void call(VM vm, In v, In x, In r, In c) {
      assert(vm->getCurrentSpace()->hasConstraintSpace());
      GecodeSpace& home = vm->getCurrentSpace()->getCstSpace();
      Gecode::IntRelType rt = atomToRelType(vm,r);
      assert(c.is<SmallInt>());
      nativeint val=c.as<SmallInt>().value();
      Gecode::linear(home,getIntArgs(vm,v),getIntVarArgs(vm,x),rt,(int)(val));
    }
  };

  class Branch: public Builtin<Branch> {
  public:
    Branch(): Builtin("branch") {}
    static void call(VM vm, In x, In bvar, In bval) {
      assert(vm->getCurrentSpace()->hasConstraintSpace());
      GecodeSpace& home = vm->getCurrentSpace()->getCstSpace();
      Gecode::IntVarBranch bvart = atomToIntVarBranch(vm,bvar); 
      Gecode::IntValBranch bvalt = atomToIntValBranch(vm,bval);
      Gecode::branch(home,getIntVarArgs(vm,x),bvart,bvalt); 
    }
  };

}; // class ModIntVarProp
} // namespace builtins
} // namespace mozart
#endif // __MODINTVARPROP_H
