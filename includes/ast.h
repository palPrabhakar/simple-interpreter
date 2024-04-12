#pragma once

#include <vector>
#include <string>

namespace tci {
class BaseAST {
 public:
  virtual ~BaseAST() = default;
  virtual std::vector<std::string> GenerateCodeStr(uint &ridx) = 0;
};

}  // namespace tci
