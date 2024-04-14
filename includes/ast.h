#pragma once

#include <string>
#include <vector>

#include "instructions.h"

namespace tci {
class BaseAST {
 public:
  virtual ~BaseAST() = default;
  virtual std::vector<std::string> GenerateCodeStr(uint &ridx) = 0;
  virtual std::vector<Instruction> GenerateCode(uint &ridx) = 0;
};

class DummyAST : public BaseAST {
 public:
  std::vector<std::string> GenerateCodeStr(uint &ridx) { return {}; }
  std::vector<Instruction> GenerateCode(uint &ridx) { return {}; }
};

}  // namespace tci
