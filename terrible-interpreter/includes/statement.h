#pragma once

#include <memory>
#include <string>

#include <iostream>

#include "ast.h"
#include "expr.h"

namespace tci {
class StatementAST : public BaseAST {
 public:
  StatementAST(std::string name, std::unique_ptr<ExprAST> expr): m_varName(name), m_expr(std::move(expr)) {}

   std::vector<std::string> GenerateCode(uint &ridx) {
     auto operations = m_expr->GenerateCode(ridx);
     operations.push_back(std::format("store r{} m@{}", m_expr->GetValue(), m_varName));
     return operations;
   }

 private:
  std::string m_varName;
  std::unique_ptr<ExprAST> m_expr;
};

class IfStatementAST : public BaseAST {
  private:
    bool else_cond = false;
};

}  // namespace tci
