#include "function.h"

namespace tci {
std::vector<std::string> ArgumentAST::GenerateCodeStr(uint &ridx) {
  return {};
}

std::vector<Instruction> ArgumentAST::GenerateCode(uint &ridx) { return {}; }

std::vector<std::string> ReturnStatementAST::GenerateCodeStr(uint &ridx) {
  auto operations = m_expr->GenerateCodeStr(ridx);
  operations.push_back(std::format("rmov r{} r0", m_expr->GetValue()));
  return operations;
}

std::vector<Instruction> ReturnStatementAST::GenerateCode(uint &ridx) {
  return {};
}

std::vector<std::string> FunctionAST::GenerateCodeStr(uint &ridx) {
  std::vector<std::string> operations;

  for(auto &node: m_body) {
    auto ops = node->GenerateCodeStr(ridx);
    std::copy(ops.begin(), ops.end(), std::back_inserter(operations));
  }

  return operations;
}

std::vector<Instruction> FunctionAST::GenerateCode(uint &ridx) { return {}; }

}  // namespace tci
