#include "interpreter_fixture.hpp"

TEST_F(InterpreterTest, TestArithmeticExpr) {
  SetCode("let x = 11 + 22;");
  EXPECT_EQ(st.GetValue("x"), 33);

  SetCode("mut x = 4 - 7;");
  EXPECT_EQ(st.GetValue("x"), -3);

  SetCode("mut x = 3*5;");
  EXPECT_EQ(st.GetValue("x"), 15);

  SetCode("mut x = 22/11;");
  EXPECT_EQ(st.GetValue("x"), 2);

  SetCode("mut x = 5 + 3*4;");
  EXPECT_EQ(st.GetValue("x"), 17);

  SetCode("mut x = 3*4 + 5*2;");
  EXPECT_EQ(st.GetValue("x"), 22);

  // SetCode("let n=1+2+3+4+5+6+7+8+9+10+11+12+13+14+15+16+17+18+19+20+21+22;");
  SetCode("let n=1+2+3+4+5+6+7+8+9+10+11+12+13+14+15+16+17+18+19+20;");
  EXPECT_EQ(st.GetValue("n"), 210);
}

TEST_F(InterpreterTest, TestExpr) {
  SetCode("let x  = 1;");
  EXPECT_TRUE(st.CheckSymbol("x"));
  EXPECT_EQ(st.GetValue("x"), 1);

  SetCode("let y = x*5;");
  EXPECT_TRUE(st.CheckSymbol("y"));
  EXPECT_EQ(st.GetValue("y"), 5);

  SetCode("let z = x+y - 5;");
  EXPECT_TRUE(st.CheckSymbol("z"));
  EXPECT_EQ(st.GetValue("z"), 1);

  SetCode("mut x = 5;");
  EXPECT_TRUE(st.CheckSymbol("x"));
  EXPECT_EQ(st.GetValue("x"), 5);

  SetCode("mut y = x/5;");
  EXPECT_TRUE(st.CheckSymbol("y"));
  EXPECT_EQ(st.GetValue("y"), 1);
}
