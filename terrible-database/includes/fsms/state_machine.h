#pragma once

#include "tokenizer.h"

namespace tdb {
class StateMachine {
public:
  virtual bool CheckTransition(Token token, std::string word) = 0;
  virtual std::string GetErrorMsg() = 0;
  virtual bool EOP() = 0;
  virtual bool CheckErrorState() = 0;
};
} // namespace tdb
