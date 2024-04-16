#pragma once

#include <algorithm>
#include <memory>

#include "ast.h"
#include "expr.h"

namespace tci {
class ReturnStatementAST : public BaseAST {
 public:
  ReturnStatementAST(std::unique_ptr<ExprAST> expr) : m_expr(std::move(expr)) {}

  std::vector<Instruction> GenerateCode(uint &ridx);

 private:
  std::unique_ptr<ExprAST> m_expr;
};

class FunctionAST : public BaseAST {
 public:
  FunctionAST(std::string name, std::vector<std::string> argumentList,
              std::vector<std::unique_ptr<BaseAST>> body)
      : m_name(name),
        m_argumentList(std::move(argumentList)),
        m_body(std::move(body)) {}

  std::vector<Instruction> GenerateCode(uint &ridx);

  std::vector<std::string> GetArgumentList() {
    return std::move(m_argumentList);
  }

  size_t GetArgumentSize() const { return m_argumentList.size(); }

  const std::string GetName() const { return m_name; }

 private:
  std::string m_name;
  std::vector<std::string> m_argumentList;
  std::vector<std::unique_ptr<BaseAST>> m_body;
};

}  // namespace tci
