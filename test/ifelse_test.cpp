#include <gtest/gtest.h>

#include "interpreter.h"
#include "parser.h"
#include "symbol_table.h"
#include "tokenizer.h"

TEST(IfElse_Test, TestIfElseStatement) {
  auto st = tci::SymbolTable();
  auto interp = tci::Interpreter(st);

  st.InsertSymbol("x");
  st.SetValue("x", 1);

  auto tokenizer =
      tci::Tokenizer("if (x  < 10) { mut x = x * 4; } else { mut x = x / 2; }");
  auto ast = tci::Parse(tokenizer, st);
  uint ridx = 1;
  auto code = ast->GenerateCode(ridx);

  EXPECT_EQ(code.size(), 13);
  EXPECT_EQ(code[0].ToString(), "load m@x r1");
  EXPECT_EQ(code[1].ToString(), "loadi 10 r2");
  EXPECT_EQ(code[2].ToString(), "lt r1 r2 r3");
  EXPECT_EQ(code[3].ToString(), "cjmp r3 p@6");
  EXPECT_EQ(code[4].ToString(), "load m@x r4");
  EXPECT_EQ(code[5].ToString(), "loadi 2 r5");
  EXPECT_EQ(code[6].ToString(), "div r4 r5 r6");
  EXPECT_EQ(code[7].ToString(), "store r6 m@x");
  EXPECT_EQ(code[8].ToString(), "jmp p@5");
  EXPECT_EQ(code[9].ToString(), "load m@x r7");
  EXPECT_EQ(code[10].ToString(), "loadi 4 r8");
  EXPECT_EQ(code[11].ToString(), "mul r7 r8 r9");
  EXPECT_EQ(code[12].ToString(), "store r9 m@x");

  interp.Interpret(std::move(code));
  EXPECT_TRUE(st.CheckSymbol("x"));
  EXPECT_EQ(st.GetValue("x"), 4);
}

TEST(IfElse_Test, TestNestedIfElseStatement) {
  auto st = tci::SymbolTable();
  auto interp = tci::Interpreter(st);

  st.InsertSymbol("x");
  st.SetValue("x", 4);

  st.InsertSymbol("y");
  st.SetValue("y", 5);
  auto tokenizer = tci::Tokenizer(
      "if (x<10) { if (y>10) { mut x = y/x; } else { mut x = x*y; } } else { "
      "mut "
      "x=x*4; }");

  auto ast = tci::Parse(tokenizer, st);
  uint ridx = 1;
  auto code = ast->GenerateCode(ridx);

  EXPECT_EQ(code.size(), 22);
  EXPECT_EQ(code[0].ToString(), "load m@x r1");
  EXPECT_EQ(code[1].ToString(), "loadi 10 r2");
  EXPECT_EQ(code[2].ToString(), "lt r1 r2 r3");
  EXPECT_EQ(code[3].ToString(), "cjmp r3 p@6");
  EXPECT_EQ(code[4].ToString(), "load m@x r4");
  EXPECT_EQ(code[5].ToString(), "loadi 4 r5");
  EXPECT_EQ(code[6].ToString(), "mul r4 r5 r6");
  EXPECT_EQ(code[7].ToString(), "store r6 m@x");
  EXPECT_EQ(code[8].ToString(), "jmp p@14");
  EXPECT_EQ(code[9].ToString(), "load m@y r7");
  EXPECT_EQ(code[10].ToString(), "loadi 10 r8");
  EXPECT_EQ(code[11].ToString(), "gt r7 r8 r9");
  EXPECT_EQ(code[12].ToString(), "cjmp r9 p@6");
  EXPECT_EQ(code[13].ToString(), "load m@x r10");
  EXPECT_EQ(code[14].ToString(), "load m@y r11");
  EXPECT_EQ(code[15].ToString(), "mul r10 r11 r12");
  EXPECT_EQ(code[16].ToString(), "store r12 m@x");
  EXPECT_EQ(code[17].ToString(), "jmp p@5");
  EXPECT_EQ(code[18].ToString(), "load m@y r13");
  EXPECT_EQ(code[19].ToString(), "load m@x r14");
  EXPECT_EQ(code[20].ToString(), "div r13 r14 r15");
  EXPECT_EQ(code[21].ToString(), "store r15 m@x");

  interp.Interpret(std::move(code));
  EXPECT_TRUE(st.CheckSymbol("x"));
  EXPECT_TRUE(st.CheckSymbol("y"));
  EXPECT_EQ(st.GetValue("x"), 20);
  EXPECT_EQ(st.GetValue("y"), 5);
}
