#include "statement.h"
#include <iterator>

#include "instructions.h"

namespace sci {
std::vector<Instruction> StatementAST::GenerateCode(uint &ridx) {
  auto operations = m_expr->GenerateCode(ridx);
  operations.emplace_back(InsCode::store, static_cast<int>(m_expr->GetValue()), m_varName);
  return operations;
}

std::vector<Instruction> IfStatementAST::GenerateCode(uint &ridx) {
  std::vector<Instruction> operations;

  auto expr_arr = m_cexpr->GenerateCode(ridx);
  std::copy(expr_arr.begin(), expr_arr.end(), std::back_inserter(operations));

  std::vector<Instruction> fb;
  for (auto &node : false_branch) {
    auto ops = node->GenerateCode(ridx);
    std::copy(ops.begin(), ops.end(), std::back_inserter(fb));
  }

  std::vector<Instruction> tb;
  for (auto &node : true_branch) {
    auto ops = node->GenerateCode(ridx);
    std::copy(ops.begin(), ops.end(), std::back_inserter(tb));
  }

  operations.emplace_back(InsCode::cjmp, static_cast<int>(m_cexpr->GetValue()), static_cast<int>(fb.size() + 2));
  std::copy(fb.begin(), fb.end(), std::back_inserter(operations));
  operations.emplace_back(InsCode::jmp, static_cast<int>(tb.size() + 1));
  std::copy(tb.begin(), tb.end(), std::back_inserter(operations));

  return operations;
}

std::vector<Instruction> WhileStatementAST::GenerateCode(uint &ridx) {
  std::vector<Instruction> operations;

  auto expr_arr = m_cexpr->GenerateCode(ridx);
  std::copy(expr_arr.begin(), expr_arr.end(), std::back_inserter(operations));

  std::vector<Instruction> body;
  for (auto &node : m_body) {
    auto ops = node->GenerateCode(ridx);
    std::copy(ops.begin(), ops.end(), std::back_inserter(body));
  }

  operations.emplace_back(InsCode::cjmp, static_cast<int>(m_cexpr->GetValue()), 2);

  // false branch
  operations.emplace_back(InsCode::jmp, static_cast<int>(body.size() + 2));

  // loop body
  std::copy(body.begin(), body.end(), std::back_inserter(operations));
  operations.emplace_back(InsCode::jmp, -static_cast<int>(operations.size()));

  return operations;
}

}  // namespace sci
