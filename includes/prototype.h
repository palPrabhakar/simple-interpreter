#pragma once

#include <string>
#include <vector>

#include "instructions.h"

namespace tci {
class FunctionPrototype {
 public:
  FunctionPrototype(std::vector<std::string> args, std::vector<Instruction> fn)
      : m_args(std::move(args)), m_fn(std::move(fn)) {}

 private:
  std::vector<std::string> m_args;
  std::vector<Instruction> m_fn;
};
}  // namespace tci
