#pragma once

#include <string>
#include <vector>

#include "instructions.h"

namespace tci {
class FunctionPrototype {
 public:
  FunctionPrototype(std::vector<std::string> args, std::vector<Instruction> fn)
      : m_args(std::move(args)), m_fn(std::move(fn)) {}

  FunctionPrototype(std::vector<std::string> args, std::vector<std::string> sfn)
      : m_args(std::move(args)), m_sfn(std::move(sfn)) {}

  std::vector<Instruction> GetInstructions() const { return m_fn; }
  std::vector<std::string> GetInstructionsStr() const { return m_sfn; }
  std::vector<std::string> GetArgsStr() const { return m_args; }

 private:
  std::vector<std::string> m_args;
  std::vector<Instruction> m_fn;
  std::vector<std::string> m_sfn;
};

}  // namespace tci
