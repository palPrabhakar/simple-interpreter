#pragma once

#include <memory>

#include "ast.h"
#include "tokens.h"
#include "symbol_table.h"

namespace tci {

class Expr : public AST {
 public:
  virtual ~Expr() = default;
  virtual double Operate() = 0;
};

class ValueExpr : public Expr {
 public:
  ValueExpr(double val) : m_val(val) {}
  double Operate() { return m_val; }
 private:
  double m_val;
};

class VarExpr: public Expr {
 public:
  VarExpr(std::string name) : m_name(name) {}
  double Operate() {
    auto &st = SymbolTable::GetInstance();
    return st.symbols[m_name];
  }

 private:
  std::string m_name;
};

class OpExpr : public Expr {
 public:
  virtual double Operate() = 0;

  void SetLhs(std::unique_ptr<Expr> lhs) { this->lhs = std::move(lhs); }

  void SetRhs(std::unique_ptr<Expr> rhs) { this->rhs = std::move(rhs); }

  virtual const int GetPrecedence() const = 0;

 protected:
  std::unique_ptr<Expr> lhs;
  std::unique_ptr<Expr> rhs;
};

class AddOp : public OpExpr {
 public:
  double Operate() { return this->lhs->Operate() + this->rhs->Operate(); }
  const int GetPrecedence() const { return 20; }
};

class SubOp : public OpExpr {
 public:
  double Operate() { return this->lhs->Operate() - this->rhs->Operate(); }
  const int GetPrecedence() const { return 20; }
};

class MulOp : public OpExpr {
 public:
  double Operate() { return this->lhs->Operate() * this->rhs->Operate(); }

  const int GetPrecedence() const { return 40; }
};

class DivOp : public OpExpr {
 public:
  double Operate() { return this->lhs->Operate() / this->rhs->Operate(); }

  const int GetPrecedence() const { return 40; }
};

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

class MakeOpExpr {
 public:
  static std::unique_ptr<OpExpr> GetOpExpr(Token token) {
    switch (token) {
      case Add:
        return std::make_unique<AddOp>();
      case Sub:
        return std::make_unique<SubOp>();
      case Mul:
        return std::make_unique<MulOp>();
      case Div:
        return std::make_unique<DivOp>();
      default:
        throw;
    }
  }
};

}  // namespace tci
