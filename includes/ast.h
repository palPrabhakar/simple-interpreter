#pragma once

#include <string>
#include <vector>

#include "instructions.h"

namespace tci {
class BaseAST {
 public:
  virtual ~BaseAST() = default;
  virtual std::vector<Instruction> GenerateCode(uint &ridx) = 0;
};

class DummyAST : public BaseAST {
 public:
  std::vector<Instruction> GenerateCode(uint &ridx) { return {}; }
};

}  // namespace tci
