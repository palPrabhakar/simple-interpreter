#pragma once

#include <gtest/gtest.h>

#include "interpreter.h"
#include "optimizers/register_alloc.hpp"
#include "parser.h"
#include "symbol_table.h"
#include "tokenizer.h"

class InterpreterTest : public ::testing::Test {
 protected:
  virtual void SetUp() {}

  void SetCode(std::string str) {
    tokenizer.ResetTokenizer(str);
    CodeGen();
  }

  void CodeGen() {
    auto ast = sci::Parse(tokenizer, st);
    if (ast) {
      auto code = ast->GenerateCode(st);
      code = sci::do_register_alloc(std::move(code));
      interp.Interpret(std::move(code));
    }
  }

  virtual void TearDown() {}

  sci::Tokenizer tokenizer{""};
  sci::SymbolTable st;
  sci::Interpreter interp{st};
};
