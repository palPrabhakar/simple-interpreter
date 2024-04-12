#pragma once

#include <vector>
#include <string>
#include "instructions.h"

namespace tci {
class BaseAST {
 public:
  virtual ~BaseAST() = default;
  virtual std::vector<std::string> GenerateCodeStr(uint &ridx) = 0;
  virtual std::vector<Instruction> GenerateCode(uint &ridx) = 0;
};

}  // namespace tci
