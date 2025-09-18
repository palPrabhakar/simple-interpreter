#include <format>

#include "function.h"
#include "instructions.h"
#include "symbol_table.h"

namespace sci {
std::vector<Instruction> ReturnStatementAST::GenerateCode(SymbolTable &st) {
  auto operations = m_expr->GenerateCode(st);
  operations.emplace_back(InsCode::rmov, static_cast<int>(m_expr->GetReg()),
                          static_cast<int>(0));
  operations.emplace_back(InsCode::ret);
  return operations;
}

std::vector<Instruction> FunctionAST::GenerateCode(SymbolTable &st) {
  std::vector<Instruction> operations;

  // TODO:
  for (auto arg : m_arguments) {
    operations.emplace_back(InsCode::store, static_cast<int>(st.GetNewRegId()), arg);
  }

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

  // save all registers
  for(int i = 1; i < static_cast<int>(st.GetRegCount()); ++i) {
    operations.emplace_back(InsCode::store, i, std::format("_$r{}", i));
  }

  int reg = 1;
  for (auto &expr : m_args) {
    operations.emplace_back(InsCode::rmov, static_cast<int>(expr->GetReg()), reg++);
  }

  operations.emplace_back(InsCode::call, m_name);

  // restore all registers
  for(int i = 1; i < static_cast<int>(st.GetRegCount()); ++i) {
    operations.emplace_back(InsCode::load, std::format("_$r{}", i), i);
  }

  return operations;
}

}  // namespace sci
