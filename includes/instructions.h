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
  Instruction(InsCode _op, std::variant<int, double, std::string> _i0,
              std::variant<int, double, std::string> _i1 = {},
              std::variant<int, double, std::string> _i2 = {})
      : op(_op), i0(_i0), i1(_i1), i2(_i2) {}

  // Instruction(InsCode _op, std::variant<int, double, std::string> _i0,
  //             std::variant<int, double, std::string> _i1)
  //     : op(_op), i0(_i0), i1(_i1) {}

  // Instruction(InsCode _op, std::variant<int, double, std::string> _i0)
  //     : op(_op), i0(_i0) {}

  InsCode op;
  std::variant<int, double, std::string> i0;
  std::variant<int, double, std::string> i1;
  std::variant<int, double, std::string> i2;
};
}  // namespace tci
