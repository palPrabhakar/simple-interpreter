#include <gtest/gtest.h>

#include "parser.h"
#include "symbol_table.h"
#include "tokenizer.h"

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

TEST(IR_Test, TestIfStatement) {
  auto st = tci::SymbolTable();
  st.InsertSymbol("x");

  auto tokenizer =
      tci::Tokenizer("if (x  < 10) { mut x = x * 4; } else { mut x = x / 2; }");
  auto ast = tci::Parse(tokenizer, st, true);
  uint ridx = 1;
  auto code = ast->GenerateCodeStr(ridx);

  EXPECT_EQ(code.size(), 13);
  EXPECT_EQ(code[0], "load m@x r1");
  EXPECT_EQ(code[1], "loadi 10 r2");
  EXPECT_EQ(code[2], "lt r1 r2 r3");
  EXPECT_EQ(code[3], "cjmp r3 p@6");
  EXPECT_EQ(code[4], "load m@x r4");
  EXPECT_EQ(code[5], "loadi 2 r5");
  EXPECT_EQ(code[6], "div r4 r5 r6");
  EXPECT_EQ(code[7], "store r6 m@x");
  EXPECT_EQ(code[8], "jmp p@5");
  EXPECT_EQ(code[9], "load m@x r7");
  EXPECT_EQ(code[10], "loadi 4 r8");
  EXPECT_EQ(code[11], "mul r7 r8 r9");
  EXPECT_EQ(code[12], "store r9 m@x");

  st.InsertSymbol("y");
  tokenizer.ResetTokenizer(
      "if (x<10) { if (y>10) { mut x = y/x; } else { mut x = x*y; } } else { "
      "mut "
      "x=x*4; }");

  ast = tci::Parse(tokenizer, st, true);
  ridx = 1;
  code = ast->GenerateCodeStr(ridx);

  EXPECT_EQ(code.size(), 22);
  EXPECT_EQ(code[0], "load m@x r1");
  EXPECT_EQ(code[1], "loadi 10 r2");
  EXPECT_EQ(code[2], "lt r1 r2 r3");
  EXPECT_EQ(code[3], "cjmp r3 p@6");
  EXPECT_EQ(code[4], "load m@x r4");
  EXPECT_EQ(code[5], "loadi 4 r5");
  EXPECT_EQ(code[6], "mul r4 r5 r6");
  EXPECT_EQ(code[7], "store r6 m@x");
  EXPECT_EQ(code[8], "jmp p@14");
  EXPECT_EQ(code[9], "load m@y r7");
  EXPECT_EQ(code[10], "loadi 10 r8");
  EXPECT_EQ(code[11], "gt r7 r8 r9");
  EXPECT_EQ(code[12], "cjmp r9 p@6");
  EXPECT_EQ(code[13], "load m@x r10");
  EXPECT_EQ(code[14], "load m@y r11");
  EXPECT_EQ(code[15], "mul r10 r11 r12");
  EXPECT_EQ(code[16], "store r12 m@x");
  EXPECT_EQ(code[17], "jmp p@5");
  EXPECT_EQ(code[18], "load m@y r13");
  EXPECT_EQ(code[19], "load m@x r14");
  EXPECT_EQ(code[20], "div r13 r14 r15");
  EXPECT_EQ(code[21], "store r15 m@x");
}
