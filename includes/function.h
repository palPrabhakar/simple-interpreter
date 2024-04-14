#pragma once

#include <algorithm>
#include <memory>

#include "ast.h"
#include "expr.h"

namespace tci {
class ReturnStatementAST : public BaseAST {
 public:
  ReturnStatementAST(std::unique_ptr<ExprAST> expr) : m_expr(std::move(expr)) {}

  std::vector<std::string> GenerateCodeStr(uint &ridx);
  std::vector<Instruction> GenerateCode(uint &ridx);

 private:
  std::unique_ptr<ExprAST> m_expr;
};

class ArgumentAST : public BaseAST {
 public:
  ArgumentAST(std::string name) : m_name(name) {}

  std::vector<std::string> GenerateCodeStr(uint &ridx);
  std::vector<Instruction> GenerateCode(uint &ridx);

 private:
  std::string m_name;
};

class FunctionAST : public BaseAST {
 public:
  FunctionAST(std::vector<std::string> argumentList,
              std::vector<std::unique_ptr<BaseAST>> body)
      : m_argumentList(std::move(argumentList)), m_body(std::move(body)) {}

  std::vector<std::string> GenerateCodeStr(uint &ridx);
  std::vector<Instruction> GenerateCode(uint &ridx);

  std::vector<std::string> GetArgumentList() {
    return std::move(m_argumentList);
  }

  size_t GetArgumentSize() const {
    return m_argumentList.size();
  }

 private:
  std::vector<std::string> m_argumentList;
  std::vector<std::unique_ptr<BaseAST>> m_body;
};


}  // namespace tci