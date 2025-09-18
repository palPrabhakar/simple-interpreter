#include <gtest/gtest.h>
#include "interpreter_fixture.hpp"

TEST_F(InterpreterTest, TestAddl) {
  SetCode("let x = 11+22;");
  EXPECT_EQ(interp.GetRegValue(st.GetReg("x")), 33);
}

TEST_F(InterpreterTest, TestSubl) {
  SetCode("let x = 4-7;");
  EXPECT_EQ(interp.GetRegValue(st.GetReg("x")), -3);
}

TEST_F(InterpreterTest, TestMull) {
  SetCode("let x = 3*5;");
  EXPECT_EQ(interp.GetRegValue(st.GetReg("x")), 15);
}

TEST_F(InterpreterTest, TestDivl) {
  SetCode("let x = 22/11;");
  EXPECT_EQ(interp.GetRegValue(st.GetReg("x")), 2);
}

TEST_F(InterpreterTest, TestComl) {
  SetCode("let n=1+2+3+4+5+6+7+8+9+10+11+12+13+14+15+16+17+18+19+20;");
  EXPECT_EQ(interp.GetRegValue(st.GetReg("n")), 210);
}

TEST_F(InterpreterTest, TestExpr) {
  SetCode("let x  = 1;");
  SetCode("let y = x*5;");
  SetCode("let z = x+y - 5;");
  SetCode("mut x = 5;");
  SetCode("mut y = x/5;");
  EXPECT_EQ(interp.GetRegValue(st.GetReg("z")), 1);
  EXPECT_EQ(interp.GetRegValue(st.GetReg("x")), 5);
  EXPECT_EQ(interp.GetRegValue(st.GetReg("y")), 1);
}
