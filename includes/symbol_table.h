#pragma once

#include <string>
#include <unordered_map>
#include <variant>

#include "tokens.h"

namespace tci {
class SymbolTable {
  public:
  SymbolTable(const SymbolTable * parent=nullptr) : m_parent(parent) {}

  SymbolTable(const SymbolTable& other) = delete;
  void operator=(const SymbolTable&) = delete;

  bool CheckSymbol(std::string) const;

  std::unordered_map<std::string, double> symbols;
  const SymbolTable *const m_parent;
};
}  // namespace tci
