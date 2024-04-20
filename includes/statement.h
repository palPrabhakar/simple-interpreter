#pragma once

#include <iostream>
#include <memory>
#include <string>

#include "ast.h"
#include "expr.h"

namespace sci {
class StatementAST : public BaseAST {
 public:
  StatementAST(std::string name, std::unique_ptr<ExprAST> expr)
      : m_varName(name), m_expr(std::move(expr)) {}

  std::vector<Instruction> GenerateCode(uint &ridx);

 private:
  std::string m_varName;
  std::unique_ptr<ExprAST> m_expr;
};

class IfStatementAST : public BaseAST {
 public:
  IfStatementAST(std::unique_ptr<ExprAST> cexpr) : m_cexpr(std::move(cexpr)) {}

  void SetTrueBranch(std::vector<std::unique_ptr<BaseAST>> tbranch) {
    true_branch = std::move(tbranch);
  }

  void SetFalseBranch(std::vector<std::unique_ptr<BaseAST>> fbranch) {
    false_branch = std::move(fbranch);
  }

  std::vector<Instruction> GenerateCode(uint &ridx);

 private:
  std::unique_ptr<ExprAST> m_cexpr;
  std::vector<std::unique_ptr<BaseAST>> true_branch;
  std::vector<std::unique_ptr<BaseAST>> false_branch;
};

class WhileStatementAST : public BaseAST {
 public:
  WhileStatementAST(std::unique_ptr<ExprAST> expr,
                    std::vector<std::unique_ptr<BaseAST>> body)
      : m_cexpr(std::move(expr)), m_body(std::move(body)) {}

  std::vector<Instruction> GenerateCode(uint &ridx);

 private:
  std::unique_ptr<ExprAST> m_cexpr;
  std::vector<std::unique_ptr<BaseAST>> m_body;
};

}  // namespace sci
