#include "statement.h"

#include <cmath>
#include <iterator>
#include <variant>

#include "expr.h"
#include "instructions.h"
#include "symbol_table.h"

namespace sci {

std::vector<Instruction> StatementAST::GenerateCode(SymbolTable &st) {
    std::vector<Instruction> operations;
    // only do store if it's a global store
    auto *val = dynamic_cast<ValueAST *>(m_expr.get());
    if (val) {
        if (!st.CheckSymbol<true>(m_varName)) {
            operations.emplace_back(InsCode::storei, val->GetLiteralValue(),
                                    m_varName);
        } else if (st.GetReg(m_varName) == -1) {
            int id = st.GetNewRegId();
            operations.emplace_back(InsCode::loadi, val->GetLiteralValue(), id);
            st.SetReg(m_varName, id);
        } else {
            // variable already exist in some register but it's
            // being assigned a new value by literal assignment.
            val->GenerateCode(st);
            operations.emplace_back(InsCode::rmov, val->GetReg(),
                                    st.GetReg(m_varName));
        }
    } else {
        int reg = st.GetReg(m_varName);
        if (!st.CheckSymbol<true>(m_varName)) {
            operations = m_expr->GenerateCode(st);
            operations.emplace_back(InsCode::store, m_expr->GetReg(),
                                    m_varName);
        }

        auto *op = dynamic_cast<OpAST *>(m_expr.get());
        if (op) {
            if (reg != -1) {
                op->SetReg(reg);
            }
            operations = op->GenerateCode(st);

            if (reg == -1) {
                st.SetReg(m_varName, op->GetReg());
            }
        } else {
            m_expr->GenerateCode(st);

            if (reg == -1) {
                reg = st.GetNewRegId();
                st.SetReg(m_varName, reg);
            }

            operations.emplace_back(InsCode::rmov, m_expr->GetReg(), reg);
        }
    }

    return operations;
}

std::vector<Instruction> IfStatementAST::GenerateCode(SymbolTable &st) {
    std::vector<Instruction> operations;

    auto expr_arr = m_cexpr->GenerateCode(st);
    std::copy(expr_arr.begin(), expr_arr.end(), std::back_inserter(operations));

    std::vector<Instruction> fb;
    for (auto &node : m_fb) {
        auto ops = node->GenerateCode(st);
        std::copy(ops.begin(), ops.end(), std::back_inserter(fb));
    }

    std::vector<Instruction> tb;
    for (auto &node : m_tb) {
        auto ops = node->GenerateCode(st);
        std::copy(ops.begin(), ops.end(), std::back_inserter(tb));
    }

    operations.emplace_back(InsCode::cjmp, m_cexpr->GetReg(),
                            static_cast<int>(fb.size() + 2));
    std::copy(fb.begin(), fb.end(), std::back_inserter(operations));
    operations.emplace_back(InsCode::jmp, static_cast<int>(tb.size() + 1));
    std::copy(tb.begin(), tb.end(), std::back_inserter(operations));

    return operations;
}

std::vector<Instruction> WhileStatementAST::GenerateCode(SymbolTable &st) {
    std::vector<Instruction> operations;

    auto expr_arr = m_cexpr->GenerateCode(st);
    std::copy(expr_arr.begin(), expr_arr.end(), std::back_inserter(operations));

    int condl_pos = operations.size();
    operations.emplace_back(InsCode::cjmp, m_cexpr->GetReg(), 2);

    // false branch
    operations.emplace_back(InsCode::jmp, Label::loop_end);
    for (auto &node : m_body) {
        auto ops = node->GenerateCode(st);
        std::copy(ops.begin(), ops.end(), std::back_inserter(operations));
    }
    operations.emplace_back(
        InsCode::jmp, -static_cast<int>(operations.size()) + condl_pos - 1);

    // for break/continue statements and loop-end jmp lbl resolution
    for (size_t i = 0; i < operations.size(); ++i) {
        if (operations[i].op == InsCode::jmp) {
            if (std::holds_alternative<std::string>(operations[i].i0)) {
                auto lbl = std::get<std::string>(operations[i].i0);
                if (lbl == Label::loop_end) {
                    operations[i].i0.emplace<int>(
                        static_cast<int>(operations.size() - i));
                } else {
                    // continue
                    operations[i].i0.emplace<int>(static_cast<int>(-i) +
                                                  condl_pos - 1);
                }
            }
        }
    }

    return operations;
}

std::vector<Instruction> PrintStatementAST::GenerateCode(SymbolTable &st) {
    std::vector<Instruction> operations;

    for (auto &expr : m_op) {
        auto ops = expr->GenerateCode(st);
        std::copy(ops.begin(), ops.end(), std::back_inserter(operations));
        operations.emplace_back(InsCode::print,
                                static_cast<int>(expr->GetReg()));
    }

    return operations;
}

}  // namespace sci
