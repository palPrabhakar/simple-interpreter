#pragma once

#include <memory>
#include <string>
#include <format>


#include "ast.h"
#include "symbol_table.h"
#include "tokens.h"

namespace tci {

class ExprAST : public BaseAST {
 public:
  virtual ~ExprAST() = default;
  virtual std::string GetValue() = 0;
  virtual void SetLhs(std::unique_ptr<ExprAST> lhs) {}
  virtual void SetRhs(std::unique_ptr<ExprAST> rhs) {}
  virtual const int GetPrecedence() const { return 0; }
};

class ValueAST : public ExprAST {
 public:
  ValueAST(std::string val) : m_val(val) {}

  std::vector<std::string> GenerateCode() {
    return {};
  }

  std::string GetValue() {
    return m_val;
  }

 private:
  std::string m_val;
};

class VarAST : public ExprAST {
 public:
  VarAST(std::string name) : m_name(name) {}

  std::vector<std::string> GenerateCode() {
    return {};
  }

  std::string GetValue() {
    return m_name;
  }

 private:
  std::string m_name;
};

class OpAST : public ExprAST {
 public:
  OpAST(Token op, std::string sop) : m_op(op), m_sop(sop) {}

  std::vector<std::string> GenerateCode() {
    std::vector<std::string> operations;
    auto left_tree = lhs->GenerateCode();
    auto right_tree = rhs->GenerateCode();
    for(auto &op: left_tree) {
      operations.push_back(std::move(op));
    }

    for(auto &op: right_tree) {
      operations.push_back(std::move(op));
    }

    operations.push_back(std::format("t0 = {} {} {}", lhs->GetValue(), m_sop, rhs->GetValue()));

    return operations;
  }

  std::string GetValue() {
    return "t0";
  }

  void SetLhs(std::unique_ptr<ExprAST> lhs) { this->lhs = std::move(lhs); }

  void SetRhs(std::unique_ptr<ExprAST> rhs) { this->rhs = std::move(rhs); }

  const int GetPrecedence() const  {
    switch(m_op) {
      case Add:
      case Sub:
        return 20;
      case Mul:
      case Div:
        return 40;
      default:
        return 0;
    }
  }

 protected:
  Token m_op;
  std::string m_sop;
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
