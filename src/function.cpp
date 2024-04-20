#include "function.h"

#include <iostream>

#include "instructions.h"

namespace sci {
std::vector<Instruction> ReturnStatementAST::GenerateCode(uint &ridx) {
  auto operations = m_expr->GenerateCode(ridx);
  operations.emplace_back(InsCode::ret);
  return operations;
}

std::vector<Instruction> FunctionAST::GenerateCode(uint &ridx) {
  std::vector<Instruction> operations;

  for (auto &node : m_body) {
    auto ops = node->GenerateCode(ridx);
    std::copy(ops.begin(), ops.end(), std::back_inserter(operations));
  }

  auto ops = m_ret->GenerateCode(ridx);
  std::copy(ops.begin(), ops.end(), std::back_inserter(operations));

  return operations;
}

std::vector<Instruction> FunctionCallAST::GenerateCode(uint &ridx) {
  std::vector<Instruction> operations;

  for (auto &expr : m_args) {
    auto ops = expr->GenerateCode(ridx);
    std::copy(ops.begin(), ops.end(), std::back_inserter(operations));
  }

  operations.emplace_back(InsCode::call, m_name);

  auto args_name = m_fn->GetArgumentList();
  for (size_t i = 0; i < m_args.size(); ++i) {
    auto &expr = m_args[i];
    auto name = args_name[i];
    operations.emplace_back(InsCode::store, static_cast<int>(expr->GetValue()),
                            name);
  }

  auto fn_code = m_fn->GenerateCode(ridx);
  reg = m_fn->GetRetVal();
  std::copy(fn_code.begin(), fn_code.end(), std::back_inserter(operations));

  return operations;
}

}  // namespace sci
