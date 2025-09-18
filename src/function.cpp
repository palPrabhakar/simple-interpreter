#include "function.h"

#include "instructions.h"
#include "symbol_table.h"

namespace sci {
std::vector<Instruction> ReturnStatementAST::GenerateCode(SymbolTable &st) {
    // TODO:
    // better codegen
    auto operations = m_expr->GenerateCode(st);
    operations.emplace_back(InsCode::rmov, static_cast<int>(m_expr->GetReg()),
                            static_cast<int>(INT_MAX));
    operations.emplace_back(InsCode::ret);
    return operations;
}

std::vector<Instruction> FunctionAST::GenerateCode(SymbolTable &st) {
    std::vector<Instruction> operations;

    for (auto &node : m_body) {
        auto ops = node->GenerateCode(st);
        std::copy(ops.begin(), ops.end(), std::back_inserter(operations));
    }

    auto ops = m_ret->GenerateCode(st);
    std::copy(ops.begin(), ops.end(), std::back_inserter(operations));

    return operations;
}

std::vector<Instruction> FunctionCallAST::GenerateCode(SymbolTable &st) {
    std::vector<Instruction> operations;

    for (auto &expr : m_args) {
        auto ops = expr->GenerateCode(st);
        std::copy(ops.begin(), ops.end(), std::back_inserter(operations));
    }

    operations.emplace_back(InsCode::push);

    int reg = 0;
    for (auto &expr : m_args) {
        operations.emplace_back(InsCode::rmov, static_cast<int>(expr->GetReg()),
                                reg++);
    }

    operations.emplace_back(InsCode::call, m_name);
    operations.emplace_back(InsCode::pop);

    m_reg = st.GetNewRegId();
    operations.emplace_back(InsCode::rmov, INT_MAX, m_reg);

    return operations;
}

}  // namespace sci
