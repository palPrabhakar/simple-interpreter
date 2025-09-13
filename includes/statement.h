#pragma once

#include <memory>
#include <string>

#include "ast.h"
#include "expr.h"

namespace sci {

template <Token token>
class LoopCtrlStatementAST : public BaseAST {
   public:
    std::vector<Instruction> GenerateCode(uint &ridx
                                          [[__maybe_unused__]]) override {
        if constexpr (token == Break) {
            return {Instruction(InsCode::jmp, Label::loop_end)};
        } else {
            return {Instruction(InsCode::jmp, Label::loop_start)};
        }
    }
};

class StatementAST : public BaseAST {
   public:
    StatementAST(std::string name, std::unique_ptr<ExprAST> expr)
        : m_varName(name), m_expr(std::move(expr)) {}

    std::vector<Instruction> GenerateCode(uint &ridx) override;

   private:
    std::string m_varName;
    std::unique_ptr<ExprAST> m_expr;
};

class IfStatementAST : public BaseAST {
   public:
    IfStatementAST(std::unique_ptr<ExprAST> cexpr,
                   std::vector<std::unique_ptr<BaseAST>> tb,
                   std::vector<std::unique_ptr<BaseAST>> fb)
        : m_cexpr(std::move(cexpr)), m_tb(std::move(tb)), m_fb(std::move(fb)) {}

    std::vector<Instruction> GenerateCode(uint &ridx) override;

   private:
    std::unique_ptr<ExprAST> m_cexpr;
    std::vector<std::unique_ptr<BaseAST>> m_tb;
    std::vector<std::unique_ptr<BaseAST>> m_fb;
};

class WhileStatementAST : public BaseAST {
   public:
    WhileStatementAST(std::unique_ptr<ExprAST> expr,
                      std::vector<std::unique_ptr<BaseAST>> body)
        : m_cexpr(std::move(expr)), m_body(std::move(body)) {}

    std::vector<Instruction> GenerateCode(uint &ridx) override;

   private:
    std::unique_ptr<ExprAST> m_cexpr;
    std::vector<std::unique_ptr<BaseAST>> m_body;
};

class PrintStatementAST : public BaseAST {
   public:
    PrintStatementAST(std::vector<std::unique_ptr<ExprAST>> op)
        : m_op(std::move(op)) {}

    std::vector<Instruction> GenerateCode(uint &ridx) override;

   private:
    std::vector<std::unique_ptr<ExprAST>> m_op;
};

}  // namespace sci
