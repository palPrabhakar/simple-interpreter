#include "symbol_table.h"

#include <cassert>

namespace tci {
bool SymbolTable::CheckSymbol(std::string name) const {
  if (!m_st.empty() && m_st.top().contains(name)) {
    return true;
  }

  if (g_symbols.contains(name)) {
    return true;
  }

  return false;
}

bool SymbolTable::CheckTopLevelSymbol(std::string name) const {
  if (!m_st.empty()) {
    return m_st.top().contains(name);
  } else {
    return g_symbols.contains(name);
  }

  return false;
}

double SymbolTable::GetValue(std::string name) const {
  if (!m_st.empty() && m_st.top().contains(name)) {
    return m_st.top().at(name);
  }

  return g_symbols.at(name);
}

void SymbolTable::SetValue(std::string name, double val) {
  if (!m_st.empty() && m_st.top().contains(name)) {
    m_st.top()[name] = val;
  }

  g_symbols[name] = val;
}

void SymbolTable::InsertSymbol(std::string name) {
  if (!m_st.empty()) {
    m_st.top().insert({name, 0});
  } else {
    g_symbols.insert({name, 0});
  }
}

void SymbolTable::InsertFunction(std::string name,
                                 std::unique_ptr<FunctionPrototype> func) {
  if (m_functions.contains(name)) {
    assert(false && "Redefinition of function.\n");
  }

  m_functions.insert({name, std::move(func)});
}

}  // namespace tci
