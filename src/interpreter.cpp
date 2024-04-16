#include "interpreter.h"

#include <format>
#include <iostream>
#include <stdexcept>

#include "instructions.h"

namespace tci {
void Interpreter::Load(Instruction ins, size_t &icounter) {
  auto vname = std::get<std::string>(ins.i0);
  auto ridx = std::get<int>(ins.i1);
  m_registers[ridx] = m_st.GetValue(vname);
  ++icounter;
}

void Interpreter::Loadi(Instruction ins, size_t &icounter) {
  auto val = std::get<double>(ins.i0);
  auto ridx = std::get<int>(ins.i1);
  m_registers[ridx] = val;
  ++icounter;
}

void Interpreter::Store(Instruction ins, size_t &icounter) {
  auto ridx = std::get<int>(ins.i0);
  auto vname = std::get<std::string>(ins.i1);
  m_st.SetValue(vname, m_registers[ridx]);
  ++icounter;
}

void Interpreter::Add(Instruction ins, size_t &icounter) {
  auto s0 = std::get<int>(ins.i0);
  auto s1 = std::get<int>(ins.i1);
  auto d = std::get<int>(ins.i2);
  m_registers[d] = m_registers[s0] + m_registers[s1];
  ++icounter;
}

void Interpreter::Sub(Instruction ins, size_t &icounter) {
  auto s0 = std::get<int>(ins.i0);
  auto s1 = std::get<int>(ins.i1);
  auto d = std::get<int>(ins.i2);
  m_registers[d] = m_registers[s0] - m_registers[s1];
  ++icounter;
}

void Interpreter::Mul(Instruction ins, size_t &icounter) {
  auto s0 = std::get<int>(ins.i0);
  auto s1 = std::get<int>(ins.i1);
  auto d = std::get<int>(ins.i2);
  m_registers[d] = m_registers[s0] * m_registers[s1];
  ++icounter;
}

void Interpreter::Div(Instruction ins, size_t &icounter) {
  auto s0 = std::get<int>(ins.i0);
  auto s1 = std::get<int>(ins.i1);
  auto d = std::get<int>(ins.i2);
  m_registers[d] = m_registers[s0] / m_registers[s1];
  ++icounter;
}

void Interpreter::Gt(Instruction ins, size_t &icounter) {
  auto s0 = std::get<int>(ins.i0);
  auto s1 = std::get<int>(ins.i1);
  auto d = std::get<int>(ins.i2);
  m_registers[d] = m_registers[s0] > m_registers[s1] ? 1.0 : 0.0;
  ++icounter;
}

void Interpreter::Lt(Instruction ins, size_t &icounter) {
  auto s0 = std::get<int>(ins.i0);
  auto s1 = std::get<int>(ins.i1);
  auto d = std::get<int>(ins.i2);
  // std::cout<<std::format("lt {} {} {}", s0, s1, d)<<std::endl;
  m_registers[d] = m_registers[s0] < m_registers[s1] ? 1.0 : 0.0;
  ++icounter;
}

void Interpreter::Lteq(Instruction ins, size_t &icounter) {
  auto s0 = std::get<int>(ins.i0);
  auto s1 = std::get<int>(ins.i1);
  auto d = std::get<int>(ins.i2);
  m_registers[d] = m_registers[s0] <= m_registers[s1] ? 1.0 : 0.0;
  ++icounter;
}

void Interpreter::Gteq(Instruction ins, size_t &icounter) {
  auto s0 = std::get<int>(ins.i0);
  auto s1 = std::get<int>(ins.i1);
  auto d = std::get<int>(ins.i2);
  m_registers[d] = m_registers[s0] >= m_registers[s1] ? 1.0 : 0.0;
  ++icounter;
}

void Interpreter::Jmp(Instruction ins, size_t &icounter) {
  auto offset = std::get<int>(ins.i0);
  icounter = icounter + offset;
  // std::cout<<std::format("jmp {}", offset)<<std::endl;
}

void Interpreter::CJmp(Instruction ins, size_t &icounter) {
  auto ridx = std::get<int>(ins.i0);
  auto offset = std::get<int>(ins.i1);
  // std::cout<<std::format("cjmp {} {}", ridx, offset)<<std::endl;
  if (m_registers[ridx] == 1.0) {
    icounter = icounter + offset;
  } else {
    ++icounter;
  }
}

void Interpreter::Call(Instruction ins, size_t &icounter) {
  auto fn_name = std::get<std::string>(ins.i0);
  m_st.PushSymbolTable();
  auto *proto = m_st.GetPrototype(fn_name);
  for(auto var: proto->GetArgsStr()) {
    m_st.InsertSymbol(var);
  }
  ++icounter;
}

void Interpreter::Rmov(Instruction ins, size_t &icounter) {
  auto r0 = std::get<int>(ins.i0);
  auto r1 = std::get<int>(ins.i1);
  m_registers[r1] = m_registers[r0];
  m_st.PopSymbolTable();
  ++icounter;
}

void Interpreter::Interpret(std::vector<Instruction> instructions) {
  size_t icounter = 0;
  while (icounter < instructions.size()) {
    auto ins = instructions[icounter];
    switch (ins.op) {
      case load:
        Load(ins, icounter);
        break;
      case loadi:
        Loadi(ins, icounter);
        break;
      case store:
        Store(ins, icounter);
        break;
      case add:
        Add(ins, icounter);
        break;
      case sub:
        Sub(ins, icounter);
        break;
      case mul:
        Mul(ins, icounter);
        break;
      case div:
        Div(ins, icounter);
        break;
      case gt:
        Gt(ins, icounter);
        break;
      case lt:
        Lt(ins, icounter);
        break;
      case gteq:
        Gteq(ins, icounter);
        break;
      case lteq:
        Lteq(ins, icounter);
        break;
      case cjmp:
        CJmp(ins, icounter);
        break;
      case jmp:
        Jmp(ins, icounter);
        break;
      case call:
        Call(ins, icounter);
        break;
      case rmov:
        Rmov(ins, icounter);
        break;
      default:
        throw std::runtime_error("Unknown instruction\n");
    }
  }
}
}  // namespace tci
