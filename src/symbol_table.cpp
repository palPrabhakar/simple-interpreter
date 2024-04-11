#include "symbol_table.h"

#include <memory>

namespace tci {
bool SymbolTable::CheckSymbol(std::string name) const {
  if (symbols.contains(name)) {
    return true;
  } else {
    if(m_parent) {
      return m_parent->CheckSymbol(name);
    } else {
      return false;
    }
  }
}
}  // namespace tci
