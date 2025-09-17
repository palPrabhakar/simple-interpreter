#pragma once

#include <memory>

#include "ast.h"
#include "expr.h"

namespace sci {
class ReturnStatementAST : public BaseAST {
   public:
    ReturnStatementAST(std::unique_ptr<ExprAST> expr)
        : m_expr(std::move(expr)) {}

    // 0 -> return register
    // store value in return register
    uint GetRetVal() const { return 0; }

    std::vector<Instruction> GenerateCode(uint& ridx) override;

   private:
    std::unique_ptr<ExprAST> m_expr;
};

class FunctionAST : public BaseAST {
   public:
    FunctionAST(std::string name, std::vector<std::string>&& args,
                std::vector<std::unique_ptr<BaseAST>>&& body,
                std::unique_ptr<ReturnStatementAST>&& ret)
        : m_name(name),
          m_arguments(std::move(args)),
          m_body(std::move(body)),
          m_ret(std::move(ret)) {}

    std::vector<Instruction> GenerateCode(uint& ridx) override;

    const std::string GetName() const { return m_name; }
    uint GetRetVal() const { return m_ret->GetRetVal(); }

   private:
    std::string m_name;
    std::vector<std::string> m_arguments;
    std::vector<std::unique_ptr<BaseAST>> m_body;
    std::unique_ptr<ReturnStatementAST> m_ret;
};

class FunctionCallAST : public ExprAST {
   public:
    FunctionCallAST(std::string name,
                    std::vector<std::unique_ptr<ExprAST>> args)
        : m_name(name), m_args(std::move(args)) {}

    // 0 -> return register
    // read value from return register
    uint GetValue() const override { return 0; }

    std::vector<Instruction> GenerateCode(uint& ridx) override;

   private:
    std::string m_name;
    std::vector<std::unique_ptr<ExprAST>> m_args;
    std::shared_ptr<FunctionAST> m_fn;
};

// Makes no sense to inherit for BaseAST
class FunctionPrototype {
   public:
    FunctionPrototype(const std::string name, const size_t size,
                      std::vector<Instruction>&& inst)
        : m_name(name), m_arg_size(size), m_instructions(std::move(inst)) {}

    size_t GetArgumentSize() const { return m_arg_size; }
    std::vector<Instruction> GetCode() { return m_instructions; }

   private:
    const std::string m_name;
    const size_t m_arg_size;
    std::vector<Instruction> m_instructions;
};

}  // namespace sci
