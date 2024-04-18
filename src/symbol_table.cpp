#include "symbol_table.h"

#include <format>
#include <stdexcept>

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
  } else {
    g_symbols[name] = val;
  }
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
    throw std::runtime_error(
        std::format("Redefinition of function {}.\n", name));
  }

  m_functions.insert({name, std::move(func)});
}

}  // namespace tci
