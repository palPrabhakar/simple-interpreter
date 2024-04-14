#include <gtest/gtest.h>
#include "symbol_table.h"
#include "tokenizer.h"
#include "parser.h"

TEST(IR_Test, TestExpr) {
  auto st = tci::SymbolTable();
  auto tokenizer = tci::Tokenizer("let x  = 1;");
  auto ast = tci::Parse(tokenizer, st, true);
  uint ridx = 1;
  auto code = ast->GenerateCodeStr(ridx);

  EXPECT_EQ(code.size(), 2);
  EXPECT_EQ(code[0], "loadi 1 r1");
  EXPECT_EQ(code[1], "store r1 m@x");

  tokenizer.ResetTokenizer("let y = x*5;");
  ast = tci::Parse(tokenizer, st, true);
  ridx = 1;
  code = ast->GenerateCodeStr(ridx);

  EXPECT_EQ(code.size(), 4);
  EXPECT_EQ(code[0], "load m@x r1");
  EXPECT_EQ(code[1], "loadi 5 r2");
  EXPECT_EQ(code[2], "mul r1 r2 r3");
  EXPECT_EQ(code[3], "store r3 m@y");

  tokenizer.ResetTokenizer("let z = x+y - 5;");
  ast = tci::Parse(tokenizer, st, true);
  ridx = 1;
  code = ast->GenerateCodeStr(ridx);
  EXPECT_EQ(code.size(), 6);
  EXPECT_EQ(code[0], "load m@x r1");
  EXPECT_EQ(code[1], "load m@y r2");
  EXPECT_EQ(code[2], "add r1 r2 r3");
  EXPECT_EQ(code[3], "loadi 5 r4");
  EXPECT_EQ(code[4], "sub r3 r4 r5");
  EXPECT_EQ(code[5], "store r5 m@z");

  tokenizer.ResetTokenizer("mut x = 5;");
  ast = tci::Parse(tokenizer, st, true);
  ridx = 1;
  code = ast->GenerateCodeStr(ridx);
  EXPECT_EQ(code.size(), 2);
  EXPECT_EQ(code[0], "loadi 5 r1");
  EXPECT_EQ(code[1], "store r1 m@x");

  tokenizer.ResetTokenizer("mut y = x/5;");
  ast = tci::Parse(tokenizer, st, true);
  ridx = 1;
  code = ast->GenerateCodeStr(ridx);

  EXPECT_EQ(code.size(), 4);
  EXPECT_EQ(code[0], "load m@x r1");
  EXPECT_EQ(code[1], "loadi 5 r2");
  EXPECT_EQ(code[2], "div r1 r2 r3");
  EXPECT_EQ(code[3], "store r3 m@y");
}

