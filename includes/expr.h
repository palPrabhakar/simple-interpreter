#pragma once

#include <memory>
#include <string>

#include "ast.h"
#include "instructions.h"
#include "tokens.h"

namespace sci {

class ExprAST : public BaseAST {
   public:
    virtual ~ExprAST() = default;
    virtual int GetReg() const { return m_reg; }

   protected:
    int m_reg;
};

class ValueAST : public ExprAST {
   public:
    ValueAST(std::string val) : m_val(std::stod(val)) {}

    std::vector<Instruction> GenerateCode(SymbolTable &st) override;

    double GetLiteralValue() const { return m_val; }

   private:
    double m_val;
};

class VarAST : public ExprAST {
   public:
    VarAST(std::string name) : m_name(name) {}

    std::vector<Instruction> GenerateCode(SymbolTable &st) override;

   private:
    std::string m_name;
};

class OpAST : public ExprAST {
   public:
    OpAST(Token op, std::string sop);

    std::vector<Instruction> GenerateCode(SymbolTable &st) override;

    void SetLhs(std::unique_ptr<ExprAST> lhs) { this->lhs = std::move(lhs); }
    void SetRhs(std::unique_ptr<ExprAST> rhs) { this->rhs = std::move(rhs); }

    int GetPrecedence() const { return m_precedence; }

    void SetReg(int reg) {
        m_dest = true;
        m_reg = reg;
    }

   protected:
    Token m_op;
    std::string m_sop;
    int m_precedence;
    InsCode m_code;
    std::unique_ptr<ExprAST> lhs;
    std::unique_ptr<ExprAST> rhs;
    bool m_dest = false;
};

}  // namespace sci
