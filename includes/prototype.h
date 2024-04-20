#pragma once

#include <string>
#include <vector>

#include "instructions.h"

namespace sci {
class FunctionPrototype {
 public:
  FunctionPrototype(std::vector<std::string> args, std::vector<std::string> symbols, std::vector<Instruction> fn)
      : m_args(std::move(args)), m_symbols(std::move(symbols)), m_fn(std::move(fn)) {}

  std::vector<Instruction> GetInstructions() const { return m_fn; }
  std::vector<std::string> GetArgsStr() const { return m_args; }
  std::vector<std::string> GetSymbols() const { return m_symbols; }

 private:
  std::vector<std::string> m_args;
  std::vector<std::string> m_symbols;
  std::vector<Instruction> m_fn;
};

}  // namespace sci
