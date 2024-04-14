#include "statement.h"
#include <iterator>

#include "instructions.h"

namespace tci {
std::vector<std::string> StatementAST::GenerateCodeStr(uint &ridx) {
  auto operations = m_expr->GenerateCodeStr(ridx);
  operations.push_back(
      std::format("store r{} m@{}", m_expr->GetValue(), m_varName));
  return operations;
}

std::vector<Instruction> StatementAST::GenerateCode(uint &ridx) {
  auto operations = m_expr->GenerateCode(ridx);
  operations.emplace_back(InsCode::store, static_cast<int>(m_expr->GetValue()), m_varName);
  return operations;
}

std::vector<std::string> IfStatementAST::GenerateCodeStr(uint &ridx) {
  std::vector<std::string> operations;

  // operations.push_back("#label: conditional_block");

  auto expr_arr = m_cexpr->GenerateCodeStr(ridx);
  std::copy(expr_arr.begin(), expr_arr.end(), std::back_inserter(operations));

  std::vector<std::string> fb;
  for (auto &node : false_branch) {
    auto ops = node->GenerateCodeStr(ridx);
    std::copy(ops.begin(), ops.end(), std::back_inserter(fb));
  }

  std::vector<std::string> tb;
  for (auto &node : true_branch) {
    auto ops = node->GenerateCodeStr(ridx);
    std::copy(ops.begin(), ops.end(), std::back_inserter(tb));
  }

  operations.push_back(
      std::format("cjmp r{} p@{}", m_cexpr->GetValue(), fb.size() + 2));

  // operations.push_back("#label: false_branch");
  std::copy(fb.begin(), fb.end(), std::back_inserter(operations));

  operations.push_back(std::format("jmp p@{}", tb.size() + 1));

  // operations.push_back("#label: true_branch");
  std::copy(tb.begin(), tb.end(), std::back_inserter(operations));

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

std::vector<std::string> WhileStatementAST::GenerateCodeStr(uint &ridx) {
  std::vector<std::string> operations;

  // operations.emplace_back("#label: conditional_block");
  auto expr_arr = m_cexpr->GenerateCodeStr(ridx);
  std::copy(expr_arr.begin(), expr_arr.end(), std::back_inserter(operations));

  std::vector<std::string> body;
  for (auto &node : m_body) {
    auto ops = node->GenerateCodeStr(ridx);
    std::copy(ops.begin(), ops.end(), std::back_inserter(body));
  }

  operations.push_back(
      std::format("cjmp r{} p@{}", m_cexpr->GetValue(), 2));
  // operations.emplace_back("#label: false-branch");
  operations.push_back(std::format("jmp p@{}", body.size() + 2));

  // operations.emplace_back("#label: loop_body");
  std::copy(body.begin(), body.end(), std::back_inserter(operations));
  operations.push_back(std::format("jmp p@-{}", operations.size()-3));

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

}  // namespace tci
