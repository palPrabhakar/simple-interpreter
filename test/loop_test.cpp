#include "interpreter_fixture.hpp"

TEST_F(InterpreterTest, TestSimpleLoop) {
  st.InsertSymbol("x");
  st.SetValue("x", 1);

  SetCode("while (x < 10) { mut x = x + 1; }");

  EXPECT_TRUE(st.CheckSymbol("x"));
  EXPECT_EQ(st.GetValue("x"), 10);
}

TEST_F(InterpreterTest, TestLoopWithCond) {
  st.InsertSymbol("x");
  st.SetValue("x", 1);
  st.InsertSymbol("i");
  st.SetValue("i", 0);

  SetCode(
      "while (i < 10) { if(i < 5) { mut x = x*2; } else { mut x = x+1; } mut i "
      "= i + 1; }");

  EXPECT_TRUE(st.CheckSymbol("x"));
  EXPECT_EQ(st.GetValue("x"), 37);
  EXPECT_TRUE(st.CheckSymbol("i"));
  EXPECT_EQ(st.GetValue("i"), 10);
}

TEST_F(InterpreterTest, TestLoopWithCondAndBreak) {
  st.InsertSymbol("x");
  st.SetValue("x", 1);
  st.InsertSymbol("i");
  st.SetValue("i", 0);

  SetCode(
      "while (i < 10) { if(i < 5) { mut x = x*2; if(x>5){break;}else{mut "
      "x=x+2;} } else { mut x = x+1; } mut i "
      "= i + 1; }");

  EXPECT_TRUE(st.CheckSymbol("x"));
  EXPECT_EQ(st.GetValue("x"), 8);
  EXPECT_TRUE(st.CheckSymbol("i"));
  EXPECT_EQ(st.GetValue("i"), 1);
}

TEST_F(InterpreterTest, TestLoopWithIfAndContinue) {
  st.InsertSymbol("x");
  st.SetValue("x", 1);
  st.InsertSymbol("i");
  st.SetValue("i", 0);

  SetCode(
      "while (i < 10) { mut i = i + 1; if(i < 5) { continue; mut x = x*2; } "
      "else { mut x = x+1; }}");

  EXPECT_TRUE(st.CheckSymbol("x"));
  EXPECT_EQ(st.GetValue("x"), 7);
  EXPECT_TRUE(st.CheckSymbol("i"));
  EXPECT_EQ(st.GetValue("i"), 10);
}

TEST_F(InterpreterTest, TestLoopWithBreak) {
  st.InsertSymbol("x");
  st.SetValue("x", 2);
  st.InsertSymbol("i");
  st.SetValue("i", 0);

  SetCode(
      "while (i < 10) { mut i = i + 1; mut x = x + i * x; break; }");

  EXPECT_TRUE(st.CheckSymbol("x"));
  EXPECT_EQ(st.GetValue("x"), 4);
  EXPECT_TRUE(st.CheckSymbol("i"));
  EXPECT_EQ(st.GetValue("i"), 1);
}

TEST_F(InterpreterTest, TestLoopWithContinue) {
  st.InsertSymbol("x");
  st.SetValue("x", 2);
  st.InsertSymbol("i");
  st.SetValue("i", 0);

  SetCode(
      "while (i < 10) { mut i = i + 1; continue; mut x = x + i * x; }");

  EXPECT_TRUE(st.CheckSymbol("x"));
  EXPECT_EQ(st.GetValue("x"), 2);
  EXPECT_TRUE(st.CheckSymbol("i"));
  EXPECT_EQ(st.GetValue("i"), 10);
}
