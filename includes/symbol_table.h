#pragma once

#include <cassert>
#include <memory>
#include <stack>
#include <string>
#include <unordered_map>

#include "prototype.h"
#include "tokens.h"

namespace tci {
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

  const Symbols& GetGlobalSymbols() const { return g_symbols; }

  bool CheckFunction(std::string name) { return m_functions.contains(name); }

  void InsertFunction(std::string name,
                      std::unique_ptr<FunctionPrototype> func);

 private:
  std::stack<Symbols> m_st;
  Symbols g_symbols;  // global symbol space
  std::unordered_map<std::string, std::unique_ptr<FunctionPrototype>>
      m_functions;
};
}  // namespace tci
