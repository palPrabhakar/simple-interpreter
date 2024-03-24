#include "symbol_table.h"

#include <memory>

namespace tci {
SymbolTable& SymbolTable::GetInstance() {
  static SymbolTable st;
  return st;
}
}  // namespace tci
