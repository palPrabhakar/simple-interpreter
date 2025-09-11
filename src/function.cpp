#include <format>

#include "function.h"
#include "instructions.h"

namespace sci {
std::vector<Instruction> ReturnStatementAST::GenerateCode(uint &ridx) {
  auto operations = m_expr->GenerateCode(ridx);
  operations.emplace_back(InsCode::rmov, static_cast<int>(m_expr->GetValue()),
                          static_cast<int>(0));
  operations.emplace_back(InsCode::ret);
  return operations;
}

std::vector<Instruction> FunctionAST::GenerateCode(uint &ridx) {
  std::vector<Instruction> operations;

  for (auto arg : m_arguments) {
    operations.emplace_back(InsCode::store, static_cast<int>(ridx++), arg);
  }

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

  // save all registers
  for(int i = 1; i < static_cast<int>(ridx); ++i) {
    operations.emplace_back(InsCode::store, i, std::format("_$r{}", i));
  }

  int reg = 1;
  for (auto &expr : m_args) {
    operations.emplace_back(InsCode::rmov, static_cast<int>(expr->GetValue()), reg++);
  }

  operations.emplace_back(InsCode::call, m_name);

  // restore all registers
  for(int i = 1; i < static_cast<int>(ridx); ++i) {
    operations.emplace_back(InsCode::load, std::format("_$r{}", i), i);
  }

  return operations;
}

}  // namespace sci
