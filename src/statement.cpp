#include "statement.h"

#include <iterator>
#include <variant>

#include "instructions.h"
#include "parser.h"
#include "tokens.h"

namespace sci {

std::vector<Instruction> StatementAST::GenerateCode(uint &ridx) {
  auto operations = m_expr->GenerateCode(ridx);
  operations.emplace_back(InsCode::store, static_cast<int>(m_expr->GetValue()),
                          m_varName);
  return operations;
}

std::vector<Instruction> IfStatementAST::GenerateCode(uint &ridx) {
  std::vector<Instruction> operations;

  auto expr_arr = m_cexpr->GenerateCode(ridx);
  std::copy(expr_arr.begin(), expr_arr.end(), std::back_inserter(operations));

  std::vector<Instruction> fb;
  for (auto &node : m_fb) {
    auto ops = node->GenerateCode(ridx);
    std::copy(ops.begin(), ops.end(), std::back_inserter(fb));
  }

  std::vector<Instruction> tb;
  for (auto &node : m_tb) {
    auto ops = node->GenerateCode(ridx);
    std::copy(ops.begin(), ops.end(), std::back_inserter(tb));
  }

  operations.emplace_back(InsCode::cjmp, static_cast<int>(m_cexpr->GetValue()),
                          static_cast<int>(fb.size() + 2));
  std::copy(fb.begin(), fb.end(), std::back_inserter(operations));
  operations.emplace_back(InsCode::jmp, static_cast<int>(tb.size() + 1));
  std::copy(tb.begin(), tb.end(), std::back_inserter(operations));

  return operations;
}

std::vector<Instruction> WhileStatementAST::GenerateCode(uint &ridx) {
  std::vector<Instruction> operations;

  auto expr_arr = m_cexpr->GenerateCode(ridx);
  std::copy(expr_arr.begin(), expr_arr.end(), std::back_inserter(operations));

  operations.emplace_back(InsCode::cjmp, static_cast<int>(m_cexpr->GetValue()),
                          2);

  // false branch
  operations.emplace_back(InsCode::jmp, Label::loop_end);
  for (auto &node : m_body) {
    auto ops = node->GenerateCode(ridx);
    std::copy(ops.begin(), ops.end(), std::back_inserter(operations));
  }
  operations.emplace_back(InsCode::jmp, -static_cast<int>(operations.size()));

  for (size_t i = 0; i < operations.size(); ++i) {
    if (operations[i].op == InsCode::jmp) {
      if (std::holds_alternative<std::string>(operations[i].i0)) {
        auto lbl = std::get<std::string>(operations[i].i0);
        if (lbl == Label::loop_end) {
          operations[i].i0.emplace<int>(
              static_cast<int>(operations.size() - i));
        } else {
          operations[i].i0.emplace<int>(
              static_cast<int>(-i));
        }
      }
    }
  }

  return operations;
}

std::vector<Instruction> PrintStatementAST::GenerateCode(uint &ridx) {
    std::vector<Instruction> operations;

    for(auto &expr: m_op) {
        auto ops = expr->GenerateCode(ridx);
        std::copy(ops.begin(), ops.end(), std::back_inserter(operations));
        operations.emplace_back(InsCode::print, static_cast<int>(expr->GetValue()));
    }

    return operations;
}

}  // namespace sci
