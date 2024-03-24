#pragma once

#include <memory>
#include <string>

#include <iostream>

#include "ast.h"
#include "expr.h"

namespace tci {
template <typename T>
class Statement : public AST {
 public:
  void Operate() {
    std::cout<<expr->Operate()<<"\n";
  }

 private:
  std::string m_varName;
  std::unique_ptr<Expr<T>> expr;
};
}  // namespace tci
