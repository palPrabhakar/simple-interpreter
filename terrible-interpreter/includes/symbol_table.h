#pragma once

#include <string>
#include <unordered_map>
#include <variant>

#include "tokens.h"

namespace tci {
class SymbolTable {
 public:
  SymbolTable(const SymbolTable& other) = delete;
  void operator=(const SymbolTable&) = delete;

  static SymbolTable& GetInstance();

  std::unordered_map<std::string, double> symbols;

 protected:
  SymbolTable() {}
};
}  // namespace tci
