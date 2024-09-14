#include <gtest/gtest.h>

#include "interpreter.h"
#include "parser.h"
#include "symbol_table.h"
#include "tokenizer.h"

TEST(Function_Test, TestSimpleFunction) {
  auto st = sci::SymbolTable();
  auto interp = sci::Interpreter(st);
  uint ridx = 1;

  auto tokenizer = sci::Tokenizer("fn double(x) { return x*2; }");
  auto ast = sci::Parse(tokenizer, st);
  auto code = ast->GenerateCode(ridx = 1);

  tokenizer.ResetTokenizer("let a = call double(1);");
  ast = sci::Parse(tokenizer, st);
  code = ast->GenerateCode(ridx = 1);
  interp.Interpret(std::move(code));
  EXPECT_TRUE(st.CheckSymbol("a"));
  EXPECT_EQ(st.GetValue("a"), 2);
}

TEST(Function_Test, TestTwoArgFunction) {
  auto st = sci::SymbolTable();
  auto interp = sci::Interpreter(st);
  uint ridx = 1;

  auto tokenizer = sci::Tokenizer("fn add(x, y) { return x + y; }");
  auto ast = sci::Parse(tokenizer, st);
  auto code = ast->GenerateCode(ridx = 1);

  tokenizer.ResetTokenizer("let a = call add(1, 2);");
  ast = sci::Parse(tokenizer, st);
  ridx = 1;
  code = ast->GenerateCode(ridx = 1);

  interp.Interpret(std::move(code));
  EXPECT_TRUE(st.CheckSymbol("a"));
  EXPECT_EQ(st.GetValue("a"), 3);
}

TEST(Function_Test, TestNestedFunction) {
  auto st = sci::SymbolTable();
  auto interp = sci::Interpreter(st);
  uint ridx = 1;

  auto tokenizer = sci::Tokenizer("fn double(x) { return x*2; }");
  auto ast = sci::Parse(tokenizer, st);
  auto code = ast->GenerateCode(ridx = 1);

  tokenizer.ResetTokenizer("fn triple(x) { return call double(x) + x; }");

  ast = sci::Parse(tokenizer, st);
  code = ast->GenerateCode(ridx = 1);

  tokenizer.ResetTokenizer("let a = call triple(1);");
  ast = sci::Parse(tokenizer, st);
  code = ast->GenerateCode(ridx = 1);

  interp.Interpret(std::move(code));
  EXPECT_TRUE(st.CheckSymbol("a"));
  EXPECT_EQ(st.GetValue("a"), 3);
}

TEST(Function_Test, TestNestedTwoArgFunction) {
  auto st = sci::SymbolTable();
  auto interp = sci::Interpreter(st);
  uint ridx = 1;

  auto tokenizer = sci::Tokenizer("fn double(x) { return x*2; }");
  auto ast = sci::Parse(tokenizer, st);
  auto code = ast->GenerateCode(ridx=1);

  tokenizer.ResetTokenizer(
      "fn add_doubles(x, y) { let xd = call double(x); let yd = call "
      "double(y); return xd + yd; }");

  ast = sci::Parse(tokenizer, st);
  code = ast->GenerateCode(ridx=1);

  tokenizer.ResetTokenizer("let a = call add_doubles(1, 2);");
  ast = sci::Parse(tokenizer, st);
  code = ast->GenerateCode(ridx=1);
  interp.Interpret(std::move(code));
  // EXPECT_EQ(st.GetGlobalSymbols().size(), 1);
  EXPECT_TRUE(st.CheckSymbol("a"));
  EXPECT_EQ(st.GetValue("a"), 6);

  tokenizer.ResetTokenizer("let b = call add_doubles(2, 4);");
  ast = sci::Parse(tokenizer, st);
  code = ast->GenerateCode(ridx=1);
  interp.Interpret(std::move(code));
  // EXPECT_EQ(st.GetGlobalSymbols().size(), 2);
  EXPECT_TRUE(st.CheckSymbol("b"));
  EXPECT_EQ(st.GetValue("b"), 12);

  tokenizer.ResetTokenizer("let c = call add_doubles(a, b);");
  ast = sci::Parse(tokenizer, st);
  code = ast->GenerateCode(ridx=1);
  interp.Interpret(std::move(code));
  // EXPECT_EQ(st.GetGlobalSymbols().size(), 3);
  EXPECT_TRUE(st.CheckSymbol("c"));
  EXPECT_EQ(st.GetValue("c"), 36);
}

TEST(Function_Test, FactorialFunction) {
  auto st = sci::SymbolTable();
  auto interp = sci::Interpreter(st);
  uint ridx = 1;

  // clang-format off
  auto tokenizer = sci::Tokenizer("fn factorial(x) {" 
                                    " if (x < 0) {" 
                                      "mut x = 0; "
                                    "} else {"
                                      "let c = x-1;"
                                      "while(c > 0) {"
                                        "mut x = x * c;"
                                        "mut c = c - 1;"
                                      "}"
                                    "}"
                                  "return x;"
                                 "}");
  // clang-format on
  auto ast = sci::Parse(tokenizer, st);
  auto code = ast->GenerateCode(ridx = 1);

  tokenizer.ResetTokenizer("let f = call factorial(0);");
  ast = sci::Parse(tokenizer, st);
  interp.Interpret(ast->GenerateCode(ridx = 1));
  EXPECT_TRUE(st.CheckSymbol("f"));
  EXPECT_EQ(st.GetValue("f"), 0);

  tokenizer.ResetTokenizer("mut f = call factorial(1);");
  ast = sci::Parse(tokenizer, st);
  interp.Interpret(ast->GenerateCode(ridx = 1));
  EXPECT_TRUE(st.CheckSymbol("f"));
  EXPECT_EQ(st.GetValue("f"), 1);

  tokenizer.ResetTokenizer("mut f = call factorial(2);");
  ast = sci::Parse(tokenizer, st);
  interp.Interpret(ast->GenerateCode(ridx = 1));
  EXPECT_TRUE(st.CheckSymbol("f"));
  EXPECT_EQ(st.GetValue("f"), 2);

  tokenizer.ResetTokenizer("mut f = call factorial(3);");
  ast = sci::Parse(tokenizer, st);
  interp.Interpret(ast->GenerateCode(ridx = 1));
  EXPECT_TRUE(st.CheckSymbol("f"));
  EXPECT_EQ(st.GetValue("f"), 6);

  tokenizer.ResetTokenizer("mut f = call factorial(4);");
  ast = sci::Parse(tokenizer, st);
  interp.Interpret(ast->GenerateCode(ridx = 1));
  EXPECT_TRUE(st.CheckSymbol("f"));
  EXPECT_EQ(st.GetValue("f"), 24);

  tokenizer.ResetTokenizer("mut f = call factorial(5);");
  ast = sci::Parse(tokenizer, st);
  interp.Interpret(ast->GenerateCode(ridx = 1));
  EXPECT_TRUE(st.CheckSymbol("f"));
  EXPECT_EQ(st.GetValue("f"), 120);
}

TEST(Function_Test, TestLargeFunction) {
  auto st = sci::SymbolTable();
  auto interp = sci::Interpreter(st);
  uint ridx = 1;

  auto tokenizer = sci::Tokenizer("fn Big_Fn(x) {"
                                    "let y = x*2;"
                                    "let z = y+x;"
                                    "let a = x+y*z;"
                                    "if(z < x*4) {"
                                      "mut z = z*x*4;"
                                    "} else {"
                                      "mut y = z;"
                                      "mut z = z/4;"
                                    "}"
                                    "let b = a*z;"
                                    "while (b > 0) {"
                                      "mut b = b-1;"
                                    "}"
                                    "return 1;"
                                  "}");
  auto ast = sci::Parse(tokenizer, st);
  auto code = ast->GenerateCode(ridx = 1);

  tokenizer.ResetTokenizer("let a = call Big_Fn(1);");
  ast = sci::Parse(tokenizer, st);
  code = ast->GenerateCode(ridx = 1);
  interp.Interpret(std::move(code));
  EXPECT_TRUE(st.CheckSymbol("a"));
  EXPECT_EQ(st.GetValue("a"), 1);
}
