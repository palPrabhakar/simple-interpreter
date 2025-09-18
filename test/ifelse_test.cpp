#include "interpreter_fixture.hpp"

TEST_F(InterpreterTest, TestIfElseStatement) {
  SetCode("let x = 1;");
  SetCode(
      "if (x  < 10) { mut x = x * 4; } else { mut x = x / 2; }");

  EXPECT_EQ(interp.GetRegValue(st.GetReg("x")), 4);
}

TEST_F(InterpreterTest, TestNestedIfElseStatement) {
  SetCode("let x = 4;");
  SetCode("let y = 5;");
  SetCode(
      "if (x<10) { if (y>10) { mut x = y/x; } else { mut x = x*y; } } else { "
      "mut "
      "x=x*4; }");

  EXPECT_EQ(interp.GetRegValue(st.GetReg("x")), 20);
  EXPECT_EQ(interp.GetRegValue(st.GetReg("y")), 5);
}
