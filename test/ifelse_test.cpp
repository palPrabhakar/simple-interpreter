#include "interpreter_fixture.hpp"

TEST_F(InterpreterTest, TestIfElseStatement) {
  st.InsertSymbol("x");
  st.SetValue("x", 1);

  SetCode(
      "if (x  < 10) { mut x = x * 4; } else { mut x = x / 2; }");

  EXPECT_TRUE(st.CheckSymbol("x"));
  EXPECT_EQ(st.GetValue("x"), 4);
}

TEST_F(InterpreterTest, TestNestedIfElseStatement) {
  st.InsertSymbol("x");
  st.SetValue("x", 4);
  st.InsertSymbol("y");
  st.SetValue("y", 5);

  SetCode(
      "if (x<10) { if (y>10) { mut x = y/x; } else { mut x = x*y; } } else { "
      "mut "
      "x=x*4; }");

  EXPECT_TRUE(st.CheckSymbol("x"));
  EXPECT_TRUE(st.CheckSymbol("y"));
  EXPECT_EQ(st.GetValue("x"), 20);
  EXPECT_EQ(st.GetValue("y"), 5);
}
