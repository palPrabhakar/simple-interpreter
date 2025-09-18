#pragma once

#include <stack>
#include <vector>

#include "instructions.h"
#include "symbol_table.h"

namespace sci {

static constexpr int reg_size = 64;

class Interpreter {
   public:
    Interpreter(SymbolTable &st) : m_reg(reg_size), m_st(st) {}

    void Interpret(std::vector<Instruction> instructions);

    double GetRegValue(size_t idx) { return m_reg[idx]; }

   private:
    // 0 idx fixed for rax (storing function return value)
    std::stack<std::vector<double>> m_registers;
    std::vector<double> m_reg;
    double m_ret;
    SymbolTable &m_st;

    inline void UpdateReg(size_t idx, double val) {
        if (idx >= m_reg.size()) {
            m_reg.resize(idx + 32);
        }
        m_reg[idx] = val;
    }

    void Load(Instruction ins, size_t &);
    void Loadi(Instruction ins, size_t &);
    void Rmov(Instruction ins, size_t &);
    void Store(Instruction ins, size_t &);
    void Storei(Instruction ins, size_t &);
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
    void Print(Instruction ins, size_t &);
    void Push(Instruction ins, size_t &);
    void Pop(Instruction ins, size_t &);
};
}  // namespace sci
