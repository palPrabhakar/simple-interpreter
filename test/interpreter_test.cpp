#include <gtest/gtest.h>
#include "symbol_table.h"
#include "tokenizer.h"
#include "parser.h"
#include "interpreter.h"

TEST(Interpreter_Test, TestExpr) {
  auto st = tci::SymbolTable();
  auto interp = tci::Interpreter(st);

  auto tokenizer = tci::Tokenizer("let x  = 1;");
  auto ast = tci::Parse(tokenizer, st);
  uint ridx = 1;
  auto code = ast->GenerateCode(ridx);
  interp.Interpret(std::move(code));

  EXPECT_TRUE(st.CheckSymbol("x"));
  EXPECT_EQ(st.GetValue("x"), 1);

  tokenizer.ResetTokenizer("let y = x*5;");
  ast = tci::Parse(tokenizer, st, true);
  ridx = 1;
  code = ast->GenerateCode(ridx);
  interp.Interpret(std::move(code));

  EXPECT_TRUE(st.CheckSymbol("y"));
  EXPECT_EQ(st.GetValue("y"), 5);

  tokenizer.ResetTokenizer("let z = x+y - 5;");
  ast = tci::Parse(tokenizer, st, true);
  ridx = 1;
  code = ast->GenerateCode(ridx);
  interp.Interpret(std::move(code));
  EXPECT_TRUE(st.CheckSymbol("z"));
  EXPECT_EQ(st.GetValue("z"), 1);
}
