#include <gtest/gtest.h>

#include "interpreter.h"
#include "parser.h"
#include "symbol_table.h"
#include "tokenizer.h"

TEST(Expr_Test, TestExpr) {
  auto st = sci::SymbolTable();
  auto interp = sci::Interpreter(st);
  uint ridx = 1;

  auto tokenizer = sci::Tokenizer("let x  = 1;");
  auto ast = sci::Parse(tokenizer, st);
  interp.Interpret(ast->GenerateCode(ridx=1));
  EXPECT_TRUE(st.CheckSymbol("x"));
  EXPECT_EQ(st.GetValue("x"), 1);

  tokenizer.ResetTokenizer("let y = x*5;");
  ast = sci::Parse(tokenizer, st);
  interp.Interpret(ast->GenerateCode(ridx=0));
  EXPECT_TRUE(st.CheckSymbol("y"));
  EXPECT_EQ(st.GetValue("y"), 5);

  tokenizer.ResetTokenizer("let z = x+y - 5;");
  ast = sci::Parse(tokenizer, st);
  interp.Interpret(ast->GenerateCode(ridx=0));
  EXPECT_TRUE(st.CheckSymbol("z"));
  EXPECT_EQ(st.GetValue("z"), 1);

  tokenizer.ResetTokenizer("mut x = 5;");
  ast = sci::Parse(tokenizer, st);
  interp.Interpret(ast->GenerateCode(ridx=0));
  EXPECT_TRUE(st.CheckSymbol("x"));
  EXPECT_EQ(st.GetValue("x"), 5);

  tokenizer.ResetTokenizer("mut y = x/5;");
  ast = sci::Parse(tokenizer, st);
  interp.Interpret(ast->GenerateCode(ridx=0));
  EXPECT_TRUE(st.CheckSymbol("y"));
  EXPECT_EQ(st.GetValue("y"), 1);
}
