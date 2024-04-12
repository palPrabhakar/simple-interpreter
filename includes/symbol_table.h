#pragma once

#include <string>
#include <unordered_map>

#include "tokens.h"

namespace tci {
struct SymbolTable {
  SymbolTable(SymbolTable * parent=nullptr) : m_parent(parent) {}

  SymbolTable(const SymbolTable& other) = delete;
  void operator=(const SymbolTable&) = delete;

  bool CheckSymbol(std::string) const;
  double GetValue(std::string) const;
  void SetValue(std::string, double);

  std::unordered_map<std::string, double> symbols;
  SymbolTable *const m_parent;
};
}  // namespace tci
