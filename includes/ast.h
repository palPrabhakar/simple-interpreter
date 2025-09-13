#pragma once

#include <string>
#include <vector>

#include "instructions.h"

namespace sci {
class BaseAST {
 public:
  virtual ~BaseAST() = default;
  virtual std::vector<Instruction> GenerateCode(uint &ridx) = 0;
};

class DummyAST : public BaseAST {
private:
  std::string m_name;
 public:
  DummyAST(std::string name): m_name(name) {}
  std::vector<Instruction> GenerateCode(uint &ridx [[__maybe_unused__]]) override { return {}; }
  std::string GetName() { return m_name; }
};

}  // namespace sci
