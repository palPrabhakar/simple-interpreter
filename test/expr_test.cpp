#include <gtest/gtest.h>

#include "interpreter.h"
#include "parser.h"
#include "symbol_table.h"
#include "tokenizer.h"

TEST(Expr_Test, TestExpr) {
  auto st = sci::SymbolTable();
  auto interp = sci::Interpreter(st);

  auto tokenizer = sci::Tokenizer("let x  = 1;");
  auto ast = sci::Parse(tokenizer, st);
  uint ridx = 1;
  auto code = ast->GenerateCode(ridx);

  EXPECT_EQ(code.size(), 2);
  EXPECT_EQ(code[0].ToString(), "loadi 1 r1");
  EXPECT_EQ(code[1].ToString(), "store r1 m@x");

  interp.Interpret(std::move(code));
  EXPECT_TRUE(st.CheckSymbol("x"));
  EXPECT_EQ(st.GetValue("x"), 1);

  tokenizer.ResetTokenizer("let y = x*5;");
  ast = sci::Parse(tokenizer, st);
  ridx = 1;
  code = ast->GenerateCode(ridx);

  EXPECT_EQ(code.size(), 4);
  EXPECT_EQ(code[0].ToString(), "load m@x r1");
  EXPECT_EQ(code[1].ToString(), "loadi 5 r2");
  EXPECT_EQ(code[2].ToString(), "mul r1 r2 r3");
  EXPECT_EQ(code[3].ToString(), "store r3 m@y");

  interp.Interpret(std::move(code));
  EXPECT_TRUE(st.CheckSymbol("y"));
  EXPECT_EQ(st.GetValue("y"), 5);

  tokenizer.ResetTokenizer("let z = x+y - 5;");
  ast = sci::Parse(tokenizer, st);
  ridx = 1;
  code = ast->GenerateCode(ridx);
  EXPECT_EQ(code.size(), 6);
  EXPECT_EQ(code[0].ToString(), "load m@x r1");
  EXPECT_EQ(code[1].ToString(), "load m@y r2");
  EXPECT_EQ(code[2].ToString(), "add r1 r2 r3");
  EXPECT_EQ(code[3].ToString(), "loadi 5 r4");
  EXPECT_EQ(code[4].ToString(), "sub r3 r4 r5");
  EXPECT_EQ(code[5].ToString(), "store r5 m@z");

  interp.Interpret(std::move(code));
  EXPECT_TRUE(st.CheckSymbol("z"));
  EXPECT_EQ(st.GetValue("z"), 1);

  tokenizer.ResetTokenizer("mut x = 5;");
  ast = sci::Parse(tokenizer, st);
  ridx = 1;
  code = ast->GenerateCode(ridx);
  EXPECT_EQ(code.size(), 2);
  EXPECT_EQ(code[0].ToString(), "loadi 5 r1");
  EXPECT_EQ(code[1].ToString(), "store r1 m@x");

  interp.Interpret(std::move(code));
  EXPECT_TRUE(st.CheckSymbol("x"));
  EXPECT_EQ(st.GetValue("x"), 5);

  tokenizer.ResetTokenizer("mut y = x/5;");
  ast = sci::Parse(tokenizer, st);
  ridx = 1;
  code = ast->GenerateCode(ridx);

  EXPECT_EQ(code.size(), 4);
  EXPECT_EQ(code[0].ToString(), "load m@x r1");
  EXPECT_EQ(code[1].ToString(), "loadi 5 r2");
  EXPECT_EQ(code[2].ToString(), "div r1 r2 r3");
  EXPECT_EQ(code[3].ToString(), "store r3 m@y");

  interp.Interpret(std::move(code));
  EXPECT_TRUE(st.CheckSymbol("y"));
  EXPECT_EQ(st.GetValue("y"), 1);
}
