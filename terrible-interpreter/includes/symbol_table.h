#pragma once

#include <string>
#include <unordered_map>
#include <variant>

#include "tokens.h"

namespace tci {
struct Symbol {
  Token token;
  std::variant<int, double , bool> val;
};

class SymbolTable {
 public:
  SymbolTable(const SymbolTable& other) = delete;
  void operator=(const SymbolTable&) = delete;

  static SymbolTable& GetInstance();

  std::unordered_map<std::string, Symbol> symbols;

 protected:
  SymbolTable() {}
};
}  // namespace tci
