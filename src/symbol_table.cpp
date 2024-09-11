#include "symbol_table.h"

namespace sci {
bool SymbolTable::CheckSymbol(std::string name) const {
  if (!m_st.empty()) {
    return m_st.back().contains(name) || m_st.front().contains(name);
  }
  return false;
}

double SymbolTable::GetValue(const std::string name) const {
  if (m_st.back().contains(name)) {
    return m_st.back().at(name);
  }
  return m_st.front().at(name);
}

void SymbolTable::SetValue(const std::string name, const double val) {
  if (m_st.back().contains(name)) {
    m_st.back()[name] = val;
  } else {
    m_st.front()[name] = val;
  }
}

void SymbolTable::InsertSymbol(const std::string name) {
  m_st.back().insert({name, 0});
}

void SymbolTable::InsertPrototype(std::string name,
                                  std::unique_ptr<FunctionPrototype> func) {
  m_functions.insert({name, std::move(func)});
}

}  // namespace sci
