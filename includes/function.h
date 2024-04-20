#pragma once

#include <algorithm>
#include <memory>

#include "ast.h"
#include "expr.h"

namespace sci {
class ReturnStatementAST : public BaseAST {
 public:
  ReturnStatementAST(std::unique_ptr<ExprAST> expr) : m_expr(std::move(expr)) {}

  std::vector<Instruction> GenerateCode(uint &ridx);

 private:
  std::unique_ptr<ExprAST> m_expr;
};

class FunctionAST : public BaseAST {
 public:
  FunctionAST(std::string name, std::vector<std::string> argumentList, std::vector<std::string> symbols,
              std::vector<std::unique_ptr<BaseAST>> body)
      : m_name(name),
        m_argumentList(std::move(argumentList)),
        m_symbols(std::move(symbols)),
        m_body(std::move(body)) {}

  std::vector<Instruction> GenerateCode(uint &ridx);

  std::vector<std::string> GetArgumentList() {
    return std::move(m_argumentList);
  }

  std::vector<std::string> GetSymbols() {
    return std::move(m_symbols);
  }

  size_t GetArgumentSize() const { return m_argumentList.size(); }

  const std::string GetName() const { return m_name; }

 private:
  std::string m_name;
  std::vector<std::string> m_argumentList;
  std::vector<std::string> m_symbols;
  std::vector<std::unique_ptr<BaseAST>> m_body;
};

}  // namespace sci
