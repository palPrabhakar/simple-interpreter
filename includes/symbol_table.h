#pragma once

#include <cassert>
#include <memory>
#include <stack>
#include <string>
#include <unordered_map>

#include "function.h"
#include "tokens.h"

namespace sci {
using Symbols = std::unordered_map<std::string, double>;

class SymbolTable {
 public:
  SymbolTable() = default;
  SymbolTable(const SymbolTable& other) = delete;
  void operator=(const SymbolTable&) = delete;

  bool CheckSymbol(std::string) const;
  bool CheckTopLevelSymbol(std::string) const;
  double GetValue(std::string) const;
  void SetValue(std::string, double);
  void InsertSymbol(std::string);

  void PushSymbolTable() { m_st.push(Symbols()); }
  void PopSymbolTable() { m_st.pop(); }

  const Symbols& GetTopLevelSymbols() const { return m_st.top(); }

  const Symbols& GetGlobalSymbols() const { return g_symbols; }

  bool CheckFunction(std::string name) { return m_functions.contains(name); }

  void InsertFunction(std::string name,
                      std::shared_ptr<FunctionAST> func);

  std::shared_ptr<FunctionAST> GetPrototype(std::string name) {
    return m_functions[name];
  }

 private:
  std::stack<Symbols> m_st;
  Symbols g_symbols;  // global symbol space
  std::unordered_map<std::string, std::shared_ptr<FunctionAST>>
      m_functions;
};
}  // namespace sci
