#include <gtest/gtest.h>

#include "interpreter.h"
#include "parser.h"
#include "symbol_table.h"
#include "tokenizer.h"

TEST(Function_Test, TestSimpleFunction) {
  auto st = sci::SymbolTable();
  auto interp = sci::Interpreter(st);

  auto tokenizer = sci::Tokenizer("fn double(x) { return x*2; }");
  auto ast = sci::Parse(tokenizer, st);
  uint ridx = 1;
  auto code = ast->GenerateCode(ridx);

  tokenizer.ResetTokenizer("let a = call double(1);");
  ast = sci::Parse(tokenizer, st);
  ridx = 1;
  code = ast->GenerateCode(ridx);

  EXPECT_EQ(code.size(), 8);
  EXPECT_EQ(code[0].ToString(), "loadi 1 r1");
  EXPECT_EQ(code[1].ToString(), "call double");
  EXPECT_EQ(code[2].ToString(), "store r1 m@x");
  EXPECT_EQ(code[3].ToString(), "load m@x r1");
  EXPECT_EQ(code[4].ToString(), "loadi 2 r2");
  EXPECT_EQ(code[5].ToString(), "mul r1 r2 r3");
  EXPECT_EQ(code[6].ToString(), "rmov r3 r0");
  EXPECT_EQ(code[7].ToString(), "store r0 m@a");

  interp.Interpret(std::move(code));
  EXPECT_TRUE(st.CheckSymbol("a"));
  EXPECT_EQ(st.GetValue("a"), 2);
}

TEST(Function_Test, TestTwoArgFunction) {
  auto st = sci::SymbolTable();
  auto interp = sci::Interpreter(st);

  auto tokenizer = sci::Tokenizer("fn add(x, y) { return x + y; }");
  auto ast = sci::Parse(tokenizer, st);
  uint ridx = 1;
  auto code = ast->GenerateCode(ridx);

  tokenizer.ResetTokenizer("let a = call add(1, 2);");
  ast = sci::Parse(tokenizer, st);
  ridx = 1;
  code = ast->GenerateCode(ridx);

  EXPECT_EQ(code.size(), 10);
  EXPECT_EQ(code[0].ToString(), "loadi 1 r1");
  EXPECT_EQ(code[1].ToString(), "loadi 2 r2");
  EXPECT_EQ(code[2].ToString(), "call add");
  EXPECT_EQ(code[3].ToString(), "store r1 m@x");
  EXPECT_EQ(code[4].ToString(), "store r2 m@y");
  EXPECT_EQ(code[5].ToString(), "load m@x r1");
  EXPECT_EQ(code[6].ToString(), "load m@y r2");
  EXPECT_EQ(code[7].ToString(), "add r1 r2 r3");
  EXPECT_EQ(code[8].ToString(), "rmov r3 r0");
  EXPECT_EQ(code[9].ToString(), "store r0 m@a");

  interp.Interpret(std::move(code));
  EXPECT_TRUE(st.CheckSymbol("a"));
  EXPECT_EQ(st.GetValue("a"), 3);
}

TEST(Function_Test, TestNestedFunction) {
  auto st = sci::SymbolTable();
  auto interp = sci::Interpreter(st);

  auto tokenizer = sci::Tokenizer("fn double(x) { return x*2; }");
  auto ast = sci::Parse(tokenizer, st);
  uint ridx = 1;
  auto code = ast->GenerateCode(ridx);

  tokenizer.ResetTokenizer("fn triple(x) { return call double(x) + x; }");

  ast = sci::Parse(tokenizer, st);
  ridx = 1;
  code = ast->GenerateCode(ridx);

  tokenizer.ResetTokenizer("let a = call triple(1);");
  ast = sci::Parse(tokenizer, st);
  ridx = 1;
  code = ast->GenerateCode(ridx);

  EXPECT_EQ(code.size(), 14);
  EXPECT_EQ(code[0].ToString(), "loadi 1 r1");
  EXPECT_EQ(code[1].ToString(), "call triple");
  EXPECT_EQ(code[2].ToString(), "store r1 m@x");
  EXPECT_EQ(code[3].ToString(), "load m@x r1");
  EXPECT_EQ(code[4].ToString(), "call double");
  EXPECT_EQ(code[5].ToString(), "store r1 m@x");
  EXPECT_EQ(code[6].ToString(), "load m@x r1");
  EXPECT_EQ(code[7].ToString(), "loadi 2 r2");
  EXPECT_EQ(code[8].ToString(), "mul r1 r2 r3");
  EXPECT_EQ(code[9].ToString(), "rmov r3 r0");
  EXPECT_EQ(code[10].ToString(), "load m@x r2");
  EXPECT_EQ(code[11].ToString(), "add r0 r2 r3");
  EXPECT_EQ(code[12].ToString(), "rmov r3 r0");
  EXPECT_EQ(code[13].ToString(), "store r0 m@a");

  interp.Interpret(std::move(code));
  EXPECT_TRUE(st.CheckSymbol("a"));
  EXPECT_EQ(st.GetValue("a"), 3);
}
