#pragma once

#include <memory>

#include "ast.h"
#include "tokens.h"

namespace tci {
template <typename T>
class Expr : public AST {
 public:
  virtual ~Expr() = default;
  virtual T Operate() = 0;
};

template <typename T>
class ValueExpr : public Expr<T> {
 public:
  ValueExpr(T val) : m_val(val) {}
  T Operate() { return m_val; }

 private:
  T m_val;
};

template <typename T, typename V>
class OpExpr : public Expr<V> {
 public:
  virtual V Operate() = 0;

  void SetLhs(std::unique_ptr<Expr<T>> lhs) { this->lhs = std::move(lhs); }

  void SetRhs(std::unique_ptr<Expr<T>> rhs) { this->rhs = std::move(rhs); }

  virtual const int GetPrecedence() const = 0;

 protected:
  std::unique_ptr<Expr<T>> lhs;
  std::unique_ptr<Expr<T>> rhs;
};

template <typename T>
class AddOp : public OpExpr<T, T> {
 public:
  T Operate() { return this->lhs->Operate() + this->rhs->Operate(); }
  const int GetPrecedence() const { return 20; }
};

template <typename T>
class SubOp : public OpExpr<T, T> {
 public:
  T Operate() { return this->lhs->Operate() - this->rhs->Operate(); }
  const int GetPrecedence() const { return 20; }
};

template <typename T>
class MulOp : public OpExpr<T, T> {
 public:
  T Operate() { return this->lhs->Operate() * this->rhs->Operate(); }

  const int GetPrecedence() const { return 40; }
};

template <typename T>
class DivOp : public OpExpr<T, T> {
 public:
  T Operate() { return this->lhs->Operate() / this->rhs->Operate(); }

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
  template <typename T, typename V = T>
  static std::unique_ptr<OpExpr<T, V>> GetOpExpr(Token token) {
    switch (token) {
      case Add:
        return std::make_unique<AddOp<T>>();
      case Sub:
        return std::make_unique<SubOp<T>>();
      case Mul:
        return std::make_unique<MulOp<T>>();
      case Div:
        return std::make_unique<DivOp<T>>();
      default:
        throw;
    }
  }
};

}  // namespace tci
