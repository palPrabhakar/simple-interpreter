#pragma once

#include <format>
#include <memory>
#include <string>

#include "ast.h"
#include "symbol_table.h"
#include "tokens.h"

namespace tci {

class ExprAST : public BaseAST {
 public:
  virtual ~ExprAST() = default;
  virtual uint GetValue() = 0;
  virtual void SetLhs(std::unique_ptr<ExprAST> lhs) {}
  virtual void SetRhs(std::unique_ptr<ExprAST> rhs) {}
  virtual const int GetPrecedence() const { return 0; }
};

class ValueAST : public ExprAST {
 public:
  ValueAST(std::string val) : m_val(val) {}

  std::vector<std::string> GenerateCode(uint &ridx);

  uint GetValue() { return reg; }

 private:
  std::string m_val;
  uint reg;
};

class VarAST : public ExprAST {
 public:
  VarAST(std::string name) : m_name(name) {}

  std::vector<std::string> GenerateCode(uint &ridx);

  uint GetValue() { return reg; }

 private:
  std::string m_name;
  uint reg;
};

class OpAST : public ExprAST {
 public:
  OpAST(Token op, std::string sop);

  std::vector<std::string> GenerateCode(uint &ridx);

  uint GetValue() { return reg; }

  void SetLhs(std::unique_ptr<ExprAST> lhs) { this->lhs = std::move(lhs); }

  void SetRhs(std::unique_ptr<ExprAST> rhs) { this->rhs = std::move(rhs); }

  const int GetPrecedence() const { return m_precedence; }

 protected:
  Token m_op;
  std::string m_sop;
  int m_precedence;
  uint reg;
  std::unique_ptr<ExprAST> lhs;
  std::unique_ptr<ExprAST> rhs;
};

// class AddOp : public OpExpr {
//  public:
//   double Operate() { return this->lhs->Operate() + this->rhs->Operate(); }
//   const int GetPrecedence() const { return 20; }
// };

// class SubOp : public OpExpr {
//  public:
//   double Operate() { return this->lhs->Operate() - this->rhs->Operate(); }
//   const int GetPrecedence() const { return 20; }
// };

// class MulOp : public OpExpr {
//  public:
//   double Operate() { return this->lhs->Operate() * this->rhs->Operate(); }

//   const int GetPrecedence() const { return 40; }
// };

// class DivOp : public OpExpr {
//  public:
//   double Operate() { return this->lhs->Operate() / this->rhs->Operate(); }

//   const int GetPrecedence() const { return 40; }
// };

// template <typename T>
// class LessOp : OpExpr<T, bool> {
//  public:
//   bool Operate() { return this->lhs->Operate() < this->rhs->Operate(); }
// };

// template <typename T>
// class MoreOp : OpExpr<T, bool> {
//  public:
//   bool Operate() { return this->lhs->Operate() > this->rhs->Operate(); }
// };

// class MakeOpExpr {
//  public:
//   static std::unique_ptr<OpExpr> GetOpExpr(Token token) {
//     switch (token) {
//       case Add:
//         return std::make_unique<AddOp>();
//       case Sub:
//         return std::make_unique<SubOp>();
//       case Mul:
//         return std::make_unique<MulOp>();
//       case Div:
//         return std::make_unique<DivOp>();
//       default:
//         throw;
//     }
//   }
// };

}  // namespace tci
