#pragma once

#include <algorithm>
#include <memory>

#include "ast.h"
#include "expr.h"

namespace sci {
class ReturnStatementAST : public BaseAST {
 public:
  ReturnStatementAST(std::unique_ptr<ExprAST> expr) : m_expr(std::move(expr)) {}

  const uint GetRetVal() const { return m_expr->GetValue(); }

  std::vector<Instruction> GenerateCode(uint &ridx);

 private:
  std::unique_ptr<ExprAST> m_expr;
};

class FunctionAST : public BaseAST {
 public:
  FunctionAST(std::string name, std::vector<std::string> argumentList,
              std::vector<std::string> symbols,
              std::vector<std::unique_ptr<BaseAST>> body,
              std::unique_ptr<ReturnStatementAST> ret)
      : m_name(name),
        m_argumentList(std::move(argumentList)),
        m_symbols(std::move(symbols)),
        m_body(std::move(body)),
        m_ret(std::move(ret)) {}

  std::vector<Instruction> GenerateCode(uint &ridx);

  std::vector<std::string> GetArgumentList() { return m_argumentList; }
  std::vector<std::string> GetSymbols() { return m_symbols; }
  size_t GetArgumentSize() const { return m_argumentList.size(); }
  const std::string GetName() const { return m_name; }
  const uint GetRetVal() const { return m_ret->GetRetVal(); }

 private:
  std::string m_name;
  std::vector<std::string> m_argumentList;
  std::vector<std::string> m_symbols;
  std::vector<std::unique_ptr<BaseAST>> m_body;
  std::unique_ptr<ReturnStatementAST> m_ret;
};

class FunctionCallAST : public ExprAST {
 public:
  FunctionCallAST(std::string name, std::vector<std::unique_ptr<ExprAST>> args,
                  std::shared_ptr<FunctionAST> fn)
      : m_name(name), m_args(std::move(args)), m_fn(std::move(fn)) {
    if (m_args.size() != m_fn->GetArgumentSize()) {
      throw std::runtime_error(std::format(
          "Argument size mismatch. Expected {} argumnets but found {}.\n",
          m_fn->GetArgumentSize(), m_args.size()));
    }
  }

  const uint GetValue() const { return reg; }

  std::vector<Instruction> GenerateCode(uint &ridx);

 private:
  uint reg;
  std::string m_name;
  std::vector<std::unique_ptr<ExprAST>> m_args;
  std::shared_ptr<FunctionAST> m_fn;
};

}  // namespace sci
