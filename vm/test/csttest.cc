#include "mozart.hh"

#include <climits>
#include <gtest/gtest.h>
#include "testutils.hh"

using namespace mozart;

class CstTest : public MozartTest {};

TEST_F(CstTest, SpaceCreation) {
  Space *currentSpace = vm->getCurrentSpace();
  GecodeSpace& cst = currentSpace->getCstSpace();
  cst.dumpSpaceInformation();
}

TEST_F(CstTest, InvalidElements) {
  UnstableNode n = SmallInt::build(vm,INT_MAX);
  EXPECT_FALSE(IntVarLike(n).isIntVarLike(vm));
  
  UnstableNode o = SmallInt::build(vm,INT_MAX-1);
  EXPECT_TRUE(IntVarLike(o).isIntVarLike(vm));

  UnstableNode p = SmallInt::build(vm,-(INT_MAX-1));
  EXPECT_TRUE(IntVarLike(p).isIntVarLike(vm));
}

TEST_F(CstTest, SmallIntIntVarLikeInterface) {
  using namespace patternmatching;

  UnstableNode n = SmallInt::build(vm,-1);

  UnstableNode min = IntVarLike(n).min(vm); 
  EXPECT_EQ_INT(-1,min);
  UnstableNode max = IntVarLike(n).max(vm);
  EXPECT_EQ_INT(-1,max);
  UnstableNode value = IntVarLike(n).value(vm);
  EXPECT_EQ_INT(-1,value);

  EXPECT_TRUE(ConstraintVar(n).assigned(vm));

  UnstableNode m = SmallInt::build(vm,-1);
  UnstableNode isInM = IntVarLike(n).isIn(vm,m);
  EXPECT_TRUE(getArgument<bool>(vm,isInM));
  
  UnstableNode o = SmallInt::build(vm,0);
  UnstableNode isInO = IntVarLike(n).isIn(vm,o);
  EXPECT_FALSE(getArgument<bool>(vm,isInO));
}

TEST_F(CstTest, IntVarLike) {
  nativeint x = -5;
  UnstableNode xNode = SmallInt::build(vm,x);
  EXPECT_TRUE(IntVarLike(xNode).isIntVarLike(vm));

  // The following test only makes sense in 64 bits architectures
  // where a nativeint can store integer bigger than INT_MIN
  nativeint out = INT_MIN + 1;
  EXPECT_FALSE(CstIntVar::validAsElement(out));
  UnstableNode outNode = SmallInt::build(vm,out);
  EXPECT_FALSE(IntVarLike(outNode).isIntVarLike(vm));

  EXPECT_RAISE(MOZART_STR("IntVarLike"),
               CstIntVar::build(vm,outNode,outNode));
}

TEST_F(CstTest, CstIntVarIntVarLikeInterface) {
  nativeint v(0);
  UnstableNode x = CstIntVar::build(vm,v);

  std::cout << "Var: " << repr(vm,x) << std::endl;
  
  UnstableNode min = IntVarLike(x).min(vm);
  std::cout << "Min: " << repr(vm,min) << std::endl;
  
  EXPECT_EQ_INT(0,min);
  UnstableNode max = IntVarLike(x).max(vm);
  EXPECT_EQ_INT(0,max);
  UnstableNode value = IntVarLike(x).value(vm);
  EXPECT_EQ_INT(0,value);

  UnstableNode m = SmallInt::build(vm,v);
  UnstableNode isInX = IntVarLike(x).isIn(vm,m);
  EXPECT_TRUE(getArgument<bool>(vm,isInX));
}

TEST_F(CstTest, ConstraintVarInterface) {
  UnstableNode n = SmallInt::build(vm,-1);
  EXPECT_TRUE(ConstraintVar(n).assigned(vm));
  
  nativeint v(0);
  UnstableNode x = CstIntVar::build(vm,v);
  EXPECT_TRUE(ConstraintVar(x).assigned(vm));
}

TEST_F(CstTest, ConstraintSpaceInterface) {
  Space *tps = vm->getCurrentSpace();
  Space* s1 = new (vm) Space(vm, tps);

  EXPECT_TRUE(vm->getCurrentSpace()->isTopLevel());
  EXPECT_TRUE(tps->isTopLevel());
  EXPECT_FALSE(s1->isTopLevel());

  s1->install();

  EXPECT_FALSE(vm->getCurrentSpace()->isTopLevel());
  EXPECT_TRUE(tps->isTopLevel());
  EXPECT_FALSE(s1->isTopLevel());

  UnstableNode rtps = ReifiedSpace::build(vm,tps);
  UnstableNode rs1 = ReifiedSpace::build(vm,s1);

  //UnstableNode m = SmallInt::build(vm,2);
  //UnstableNode M = SmallInt::build(vm,4);
  //UnstableNode x=CstIntVar::build(vm,m,M);
  nativeint v(0);
  UnstableNode x = CstIntVar::build(vm,v);

  EXPECT_FALSE(ConstraintSpace(rtps).isConstraintSpace(vm));
  EXPECT_TRUE(ConstraintSpace(rs1).isConstraintSpace(vm));

  tps->install();

  EXPECT_TRUE(vm->getCurrentSpace()->isTopLevel());
  EXPECT_TRUE(tps->isTopLevel());
  EXPECT_FALSE(s1->isTopLevel());

  std::cout << "Incio askSpace" << std::endl;
  UnstableNode rs2 = SpaceLike(rs1).askSpace(vm);
  std::cout << "fin askSpace" << std::endl;
  
  RichNode statusVar = *s1->getStatusVar();
  EXPECT_TRUE(statusVar.isTransient());

  //EXPECT_FALSE(ConstraintSpace(rtps).isConstraintSpace(vm));
  //EXPECT_TRUE(ConstraintSpace(rs1).isConstraintSpace(vm));
  //EXPECT_TRUE(ConstraintSpace(rs2).isConstraintSpace(vm));

  
  /*
  GecodeSpace& cst = ConstraintSpace(sp1).constraintSpace(vm);
  cst.dumpSpaceInformation();
  GecodeSpace& cst1 = space1->getCstSpace();
  cst1.dumpSpaceInformation();*/
}

/*TEST_F(CstTest, CstIntVarIntVarLikeInterface) {
  nativeint v(0);
  UnstableNode x = CstIntVar::build(vm,v);

  std::cout << "Var: " << repr(vm,x) << std::endl;
  
  UnstableNode min = IntVarLike(x).min(vm);
  std::cout << "Min: " << repr(vm,min) << std::endl;
  
  EXPECT_EQ_INT(0,min);
  UnstableNode max = IntVarLike(x).max(vm);
  EXPECT_EQ_INT(0,max);
  UnstableNode value = IntVarLike(x).value(vm);
  EXPECT_EQ_INT(0,value);

  UnstableNode m = SmallInt::build(vm,v);
  UnstableNode isInX = IntVarLike(x).isIn(vm,m);
  EXPECT_TRUE(BooleanValue(isInX).boolValue(vm));
}

TEST_F(CstTest, ConstraintVarInterface) {
  UnstableNode n = SmallInt::build(vm,-1);
  EXPECT_TRUE(ConstraintVar(n).assigned(vm));
  
  nativeint v(0);
  UnstableNode x = CstIntVar::build(vm,v);
  EXPECT_TRUE(ConstraintVar(x).assigned(vm));
}*/

