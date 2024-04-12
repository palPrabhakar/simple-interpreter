#include "symbol_table.h"

#include <cassert>

namespace tci {
bool SymbolTable::CheckSymbol(std::string name) const {
  if (symbols.contains(name)) {
    return true;
  } else {
    if (m_parent) {
      return m_parent->CheckSymbol(name);
    } else {
      return false;
    }
  }
}

double SymbolTable::GetValue(std::string name) const {
  assert(CheckSymbol(name) && "Symbol not found\n");

  if (symbols.contains(name)) {
    return symbols.at(name);
  } else {
    return m_parent->GetValue(name);
  }
}

void SymbolTable::SetValue(std::string name, double val) {
  assert(CheckSymbol(name) && "Symbol not found\n");

  if (symbols.contains(name)) {
    symbols[name] = val;
  } else {
    m_parent->SetValue(name, val);
  }
}
}  // namespace tci
