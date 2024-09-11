#include "instructions.h"

#include <format>
#include <stdexcept>


namespace sci {

const std::string Label::loop_start = "begin";
const std::string Label::loop_end = "end";

std::string Instruction::Load() const {
  return std::format("load m@{} r{}", std::get<std::string>(i0),
                     std::get<int>(i1));
}

std::string Instruction::Loadi() const {
  return std::format("loadi {} r{}", std::get<double>(i0), std::get<int>(i1));
}

std::string Instruction::Rmov() const {
  return std::format("rmov r{} r{}", std::get<int>(i0), std::get<int>(i1));
}

std::string Instruction::Store() const {
  return std::format("store r{} m@{}", std::get<int>(i0),
                     std::get<std::string>(i1));
}

std::string Instruction::Add() const {
  return std::format("add r{} r{} r{}", std::get<int>(i0), std::get<int>(i1),
                     std::get<int>(i2));
}

std::string Instruction::Sub() const {
  return std::format("sub r{} r{} r{}", std::get<int>(i0), std::get<int>(i1),
                     std::get<int>(i2));
}

std::string Instruction::Mul() const {
  return std::format("mul r{} r{} r{}", std::get<int>(i0), std::get<int>(i1),
                     std::get<int>(i2));
}

std::string Instruction::Div() const {
  return std::format("div r{} r{} r{}", std::get<int>(i0), std::get<int>(i1),
                     std::get<int>(i2));
}

std::string Instruction::Gt() const {
  return std::format("gt r{} r{} r{}", std::get<int>(i0), std::get<int>(i1),
                     std::get<int>(i2));
}

std::string Instruction::Lt() const {
  return std::format("lt r{} r{} r{}", std::get<int>(i0), std::get<int>(i1),
                     std::get<int>(i2));
}

std::string Instruction::Lteq() const {
  return std::format("lteq r{} r{} r{}", std::get<int>(i0), std::get<int>(i1),
                     std::get<int>(i2));
}

std::string Instruction::Gteq() const {
  return std::format("gteq r{} r{} r{}", std::get<int>(i0), std::get<int>(i1),
                     std::get<int>(i2));
}

std::string Instruction::Jmp() const {
  return std::format("jmp p@{}", std::get<int>(i0));
}

std::string Instruction::CJmp() const {
  return std::format("cjmp r{} p@{}", std::get<int>(i0), std::get<int>(i1));
}

std::string Instruction::Call() const {
  return std::format("call {}", std::get<std::string>(i0));
}

std::string Instruction::Ret() const { return std::string("ret"); }

std::string Instruction::ToString() const {
  switch (op) {
    case load:
      return Load();
    case loadi:
      return Loadi();
    case rmov:
      return Rmov();
    case store:
      return Store();
    case add:
      return Add();
    case sub:
      return Sub();
    case mul:
      return Mul();
    case div:
      return Div();
    case gt:
      return Gt();
    case lt:
      return Lt();
    case gteq:
      return Gteq();
    case lteq:
      return Lteq();
    case cjmp:
      return CJmp();
    case jmp:
      return Jmp();
    case call:
      return Call();
    case ret:
      return Ret();
    default:
      throw std::runtime_error("Unknown instruction\n");
  }
}
}  // namespace sci
