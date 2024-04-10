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

   std::vector<std::string> GenerateCode() {
     auto operations = m_expr->GenerateCode();
     operations.push_back(std::format("{} = {}", m_varName, m_expr->GetValue()));
     return operations;
   }

 private:
  std::string m_varName;
  std::unique_ptr<ExprAST> m_expr;
};
}  // namespace tci
