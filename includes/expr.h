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
    virtual uint GetValue() const = 0;
};

class ValueAST : public ExprAST {
   public:
    ValueAST(std::string val) : m_val(val) {}

    std::vector<Instruction> GenerateCode(uint &ridx) override;

    double GetLiteralValue() const { return std::stod(m_val); }

    uint GetValue() const override { return reg; }

   private:
    std::string m_val;
    uint reg;
};

class VarAST : public ExprAST {
   public:
    VarAST(std::string name) : m_name(name) {}

    std::vector<Instruction> GenerateCode(uint &ridx) override;

    uint GetValue() const override { return reg; }

   private:
    std::string m_name;
    uint reg;
};

class OpAST : public ExprAST {
   public:
    OpAST(Token op, std::string sop);

    std::vector<Instruction> GenerateCode(uint &ridx) override;

    uint GetValue() const override { return reg; }

    void SetLhs(std::unique_ptr<ExprAST> lhs) { this->lhs = std::move(lhs); }
    void SetRhs(std::unique_ptr<ExprAST> rhs) { this->rhs = std::move(rhs); }

    int GetPrecedence() const { return m_precedence; }

   protected:
    Token m_op;
    std::string m_sop;
    int m_precedence;
    InsCode m_code;
    uint reg;
    std::unique_ptr<ExprAST> lhs;
    std::unique_ptr<ExprAST> rhs;
};

}  // namespace sci
