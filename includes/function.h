#pragma once

#include "ast.h"

namespace tci {
  class FunctionAST : public BaseAST {
    public:
      FunctionAST(std::vector<std::string> argumentList): m_argumentList(std::move(argumentList)) {}

    private:
      std::vector<std::string> m_argumentList;
  };
}
