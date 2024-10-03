#pragma once

#include <cassert>
#include <deque>
#include <memory>
#include <string>
#include <unordered_map>

#include "function.h"

namespace sci {
using Symbols = std::unordered_map<std::string, double>;
using Functions =
    std::unordered_map<std::string, std::unique_ptr<FunctionPrototype>>;

// TODO
// Make it Singleton
class SymbolTable {
 public:
  SymbolTable() {
    assert(m_st.empty());
    PushSymbolTable();
  }
  SymbolTable(const SymbolTable& other) = delete;
  void operator=(const SymbolTable&) = delete;

  template <bool top_level = false>
  bool CheckSymbol(const std::string name) const {
    if (!m_st.empty()) {
      return m_st.back().contains(name) ||
             (!top_level && m_st.front().contains(name));
    }
    return false;
  }
  double GetValue(const std::string) const;
  void SetValue(const std::string, const double);
  void InsertSymbol(const std::string);

  void PushSymbolTable() { m_st.push_back(Symbols()); }
  void PopSymbolTable() { m_st.pop_back(); }

  const Symbols& GetTopLevelSymbols() const { return m_st.back(); }

  const Symbols& GetGlobalSymbols() const { return m_st.front(); }

  bool CheckFunction(const std::string name) const {
    return m_functions.contains(name);
  }

  void InsertPrototype(const std::string name,
                       std::unique_ptr<FunctionPrototype> proto);

  const std::unique_ptr<FunctionPrototype>& GetPrototype(
      const std::string name) {
    return m_functions[name];
  }

 private:
  std::deque<Symbols> m_st;
  Functions m_functions;
};
}  // namespace sci
