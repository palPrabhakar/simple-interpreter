#include "function.h"

#include "instructions.h"

namespace sci {
std::vector<Instruction> ReturnStatementAST::GenerateCode(uint &ridx) {
  auto operations = m_expr->GenerateCode(ridx);
  operations.emplace_back(InsCode::rmov, static_cast<int>(m_expr->GetValue()),
                          0);
  return operations;
}

std::vector<Instruction> FunctionAST::GenerateCode(uint &ridx) {
  std::vector<Instruction> operations;

  for (auto &node : m_body) {
    auto ops = node->GenerateCode(ridx);
    std::copy(ops.begin(), ops.end(), std::back_inserter(operations));
  }

  return operations;
}

}  // namespace sci
