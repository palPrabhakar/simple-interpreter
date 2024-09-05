#include <gtest/gtest.h>

#include "interpreter.h"
#include "parser.h"
#include "symbol_table.h"
#include "tokenizer.h"

TEST(IfElse_Test, TestIfElseStatement) {
  auto st = sci::SymbolTable();
  auto interp = sci::Interpreter(st);
  uint ridx = 1;

  st.InsertSymbol("x");
  st.SetValue("x", 1);

  auto tokenizer =
      sci::Tokenizer("if (x  < 10) { mut x = x * 4; } else { mut x = x / 2; }");
  auto ast = sci::Parse(tokenizer, st);
  auto code = ast->GenerateCode(ridx);
  interp.Interpret(std::move(code));
  EXPECT_TRUE(st.CheckSymbol("x"));
  EXPECT_EQ(st.GetValue("x"), 4);
}

TEST(IfElse_Test, TestNestedIfElseStatement) {
  auto st = sci::SymbolTable();
  auto interp = sci::Interpreter(st);
  uint ridx = 1;

  st.InsertSymbol("x");
  st.SetValue("x", 4);
  st.InsertSymbol("y");
  st.SetValue("y", 5);

  auto tokenizer = sci::Tokenizer(
      "if (x<10) { if (y>10) { mut x = y/x; } else { mut x = x*y; } } else { "
      "mut "
      "x=x*4; }");

  auto ast = sci::Parse(tokenizer, st);
  auto code = ast->GenerateCode(ridx);
  interp.Interpret(std::move(code));
  EXPECT_TRUE(st.CheckSymbol("x"));
  EXPECT_TRUE(st.CheckSymbol("y"));
  EXPECT_EQ(st.GetValue("x"), 20);
  EXPECT_EQ(st.GetValue("y"), 5);
}
