#include <gtest/gtest.h>

#include "interpreter.h"
#include "parser.h"
#include "symbol_table.h"
#include "tokenizer.h"

TEST(Loop_Test, TestSimpleLoop) {
  auto st = sci::SymbolTable();
  auto interp = sci::Interpreter(st);
  uint ridx = 1;

  st.InsertSymbol("x");
  st.SetValue("x", 1);

  auto tokenizer = sci::Tokenizer("while (x < 10) { mut x = x + 1; }");
  auto ast = sci::Parse(tokenizer, st);
  interp.Interpret(ast->GenerateCode(ridx=1));
  EXPECT_TRUE(st.CheckSymbol("x"));
  EXPECT_EQ(st.GetValue("x"), 10);
}

TEST(Loop_Test, TestLoopWithCond) {
  auto st = sci::SymbolTable();
  auto interp = sci::Interpreter(st);
  uint ridx = 1;

  st.InsertSymbol("x");
  st.SetValue("x", 1);
  st.InsertSymbol("i");
  st.SetValue("i", 0);

  auto tokenizer = sci::Tokenizer(
      "while (i < 10) { if(i < 5) { mut x = x*2; } else { mut x = x+1; } mut i "
      "= i + 1; }");
  auto ast = sci::Parse(tokenizer, st);
  interp.Interpret(ast->GenerateCode(ridx=1));
  EXPECT_TRUE(st.CheckSymbol("x"));
  EXPECT_EQ(st.GetValue("x"), 37);
  EXPECT_TRUE(st.CheckSymbol("i"));
  EXPECT_EQ(st.GetValue("i"), 10);
}

TEST(Loop_Test, TestLoopWithCondAndBreak) {
  auto st = sci::SymbolTable();
  auto interp = sci::Interpreter(st);
  uint ridx = 1;

  st.InsertSymbol("x");
  st.SetValue("x", 1);
  st.InsertSymbol("i");
  st.SetValue("i", 0);

  auto tokenizer = sci::Tokenizer(
      "while (i < 10) { if(i < 5) { mut x = x*2; if(x>5){break;}else{mut x=x+2;} } else { mut x = x+1; } mut i "
      "= i + 1; }");
  auto ast = sci::Parse(tokenizer, st);
  auto code = ast->GenerateCode(ridx);
  interp.Interpret(std::move(code));
  EXPECT_TRUE(st.CheckSymbol("x"));
  EXPECT_EQ(st.GetValue("x"), 8);
  EXPECT_TRUE(st.CheckSymbol("i"));
  EXPECT_EQ(st.GetValue("i"), 1);
}

TEST(Loop_Test, TestLoopWithIfAndContinue) {
  auto st = sci::SymbolTable();
  auto interp = sci::Interpreter(st);
  uint ridx = 1;

  st.InsertSymbol("x");
  st.SetValue("x", 1);
  st.InsertSymbol("i");
  st.SetValue("i", 0);

  auto tokenizer = sci::Tokenizer(
      "while (i < 10) { mut i = i + 1; if(i < 5) { continue; mut x = x*2; } else { mut x = x+1; }}");
  auto ast = sci::Parse(tokenizer, st);
  auto code = ast->GenerateCode(ridx);
  interp.Interpret(std::move(code));
  EXPECT_TRUE(st.CheckSymbol("x"));
  EXPECT_EQ(st.GetValue("x"), 7);
  EXPECT_TRUE(st.CheckSymbol("i"));
  EXPECT_EQ(st.GetValue("i"), 10);
}

TEST(Loop_Test, TestLoopWithBreak) {
  auto st = sci::SymbolTable();
  auto interp = sci::Interpreter(st);
  uint ridx = 1;
  
  st.InsertSymbol("x");
  st.SetValue("x", 2);
  st.InsertSymbol("i");
  st.SetValue("i", 0);

  auto tokenizer = sci::Tokenizer(
      "while (i < 10) { mut i = i + 1; mut x = x + i * x; break; }");
  auto ast = sci::Parse(tokenizer, st);
  auto code = ast->GenerateCode(ridx);
  interp.Interpret(std::move(code));
  EXPECT_TRUE(st.CheckSymbol("x"));
  EXPECT_EQ(st.GetValue("x"), 4);
  EXPECT_TRUE(st.CheckSymbol("i"));
  EXPECT_EQ(st.GetValue("i"), 1);
}

TEST(Loop_Test, TestLoopWithContinue) {
  auto st = sci::SymbolTable();
  auto interp = sci::Interpreter(st);
  uint ridx = 1;

  st.InsertSymbol("x");
  st.SetValue("x", 2);
  st.InsertSymbol("i");
  st.SetValue("i", 0);

  auto tokenizer = sci::Tokenizer(
      "while (i < 10) { mut i = i + 1; continue; mut x = x + i * x; }");
  auto ast = sci::Parse(tokenizer, st);
  auto code = ast->GenerateCode(ridx);
  interp.Interpret(std::move(code));
  EXPECT_TRUE(st.CheckSymbol("x"));
  EXPECT_EQ(st.GetValue("x"), 2);
  EXPECT_TRUE(st.CheckSymbol("i"));
  EXPECT_EQ(st.GetValue("i"), 10);
}
