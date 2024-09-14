#include <gtest/gtest.h>

#include "interpreter.h"
#include "parser.h"
#include "symbol_table.h"
#include "tokenizer.h"

TEST(Expr_Test, TestArithmeticExpr) {
  auto st = sci::SymbolTable();
  auto interp = sci::Interpreter(st);
  uint ridx = 1;

  auto tokenizer = sci::Tokenizer("let x = 11 + 22;");
  auto ast = sci::Parse(tokenizer, st);
  interp.Interpret(ast->GenerateCode(ridx=1));
  EXPECT_EQ(st.GetValue("x"), 33);

  tokenizer.ResetTokenizer("mut x = 4 - 7;");
  ast = sci::Parse(tokenizer, st);
  interp.Interpret(ast->GenerateCode(ridx=1));
  EXPECT_EQ(st.GetValue("x"), -3);

  tokenizer.ResetTokenizer("mut x = 3*5;");
  ast = sci::Parse(tokenizer, st);
  interp.Interpret(ast->GenerateCode(ridx=1));
  EXPECT_EQ(st.GetValue("x"), 15);

  tokenizer.ResetTokenizer("mut x = 22/11;");
  ast = sci::Parse(tokenizer, st);
  interp.Interpret(ast->GenerateCode(ridx=1));
  EXPECT_EQ(st.GetValue("x"), 2);

  tokenizer.ResetTokenizer("mut x = 5 + 3*4;");
  ast = sci::Parse(tokenizer, st);
  interp.Interpret(ast->GenerateCode(ridx=1));
  EXPECT_EQ(st.GetValue("x"), 17);

  tokenizer.ResetTokenizer("mut x = 3*4 + 5*2;");
  ast = sci::Parse(tokenizer, st);
  interp.Interpret(ast->GenerateCode(ridx=1));
  EXPECT_EQ(st.GetValue("x"), 22);
}

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
