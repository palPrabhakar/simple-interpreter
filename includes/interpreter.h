#pragma once

#include "instructions.h"
#include "symbol_table.h"

namespace sci {
  
static constexpr unsigned int reg_size = 32;

class Interpreter {
 public:
  Interpreter(SymbolTable &st)
      : m_st(st) {}

  void Interpret(std::vector<Instruction> instructions);

 private:
  // 0 idx fixed for rax (storing function return value)
  double m_registers[reg_size];
  SymbolTable &m_st;

  inline void UpdateReg(size_t idx, double val) {
    m_registers[idx] = val;
  }

  void Load(Instruction ins, size_t &);
  void Loadi(Instruction ins, size_t &);
  void Store(Instruction ins, size_t &);
  void Add(Instruction ins, size_t &);
  void Sub(Instruction ins, size_t &);
  void Mul(Instruction ins, size_t &);
  void Div(Instruction ins, size_t &);
  void Gt(Instruction ins, size_t &);
  void Lt(Instruction ins, size_t &);
  void Gteq(Instruction ins, size_t &);
  void Lteq(Instruction ins, size_t &);
  void Jmp(Instruction ins, size_t &);
  void CJmp(Instruction ins, size_t &);
  void Call(Instruction ins, size_t &);
  void Ret(Instruction ins, size_t &);
};
}  // namespace sci
