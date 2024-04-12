#pragma once

#include <string>
#include <variant>

namespace tci {
enum InsCode {
  loadi,
  load,
  // storei,
  store,
  add,
  sub,
  mul,
  div,
  gt,
  lt,
  gteq,
  lteq,
  cjmp,
  jmp
};

struct Instruction {
  InsCode op;
  std::variant<size_t, double, std::string> i0;
  std::variant<size_t, double, std::string> i1;
  std::variant<size_t, double, std::string> i2;
};
}  // namespace tci
