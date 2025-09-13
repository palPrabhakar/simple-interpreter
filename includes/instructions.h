#pragma once

#include <string>
#include <variant>

namespace sci {
enum InsCode {
  loadi,
  load,
  rmov,
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
  jmp,
  call,
  ret,
  print
};

struct Label {
  const static std::string loop_end;
  const static std::string loop_start;
};

class Instruction {
public:
  Instruction(InsCode _op, std::variant<int, double, std::string> _i0 = {},
              std::variant<int, double, std::string> _i1 = {},
              std::variant<int, double, std::string> _i2 = {})
      : op(_op), i0(_i0), i1(_i1), i2(_i2) {}

  InsCode op;
  std::variant<int, double, std::string> i0;
  std::variant<int, double, std::string> i1;
  std::variant<int, double, std::string> i2;

  std::string ToString() const;

 private:
  std::string Load() const;
  std::string Loadi() const;
  std::string Store() const;
  std::string Add() const;
  std::string Sub() const;
  std::string Mul() const;
  std::string Div() const;
  std::string Gt() const;
  std::string Lt() const;
  std::string Gteq() const;
  std::string Lteq() const;
  std::string Jmp() const;
  std::string CJmp() const;
  std::string Call() const;
  std::string Ret() const;
  std::string Rmov() const;
  std::string Print() const;
};
}  // namespace sci
