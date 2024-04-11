#include "expr.h"

namespace tci {

std::vector<std::string> ValueAST::GenerateCode(uint &ridx) {
  reg = ridx++;
  return {std::format("loadi {} r{}", m_val, reg)};
}

std::vector<std::string> VarAST::GenerateCode(uint &ridx) {
  reg = ridx++;
  return {std::format("load m@{} r{}", m_name, reg)};
}

OpAST::OpAST(Token op, std::string sop) : m_op(op), m_sop(sop) {
  switch (m_op) {
    case Add:
      m_sop = "add";
      m_precedence = 20;
      break;
    case Sub:
      m_sop = "sub";
      m_precedence = 20;
      break;
    case Mul:
      m_sop = "mul";
      m_precedence = 40;
      break;
    case Div:
      m_sop = "div";
      m_precedence = 40;
      break;
    default:
      m_precedence = 0;
      break;
  }
}

std::vector<std::string> OpAST::GenerateCode(uint &ridx) {
  std::vector<std::string> operations;
  auto left_tree = lhs->GenerateCode(ridx);
  auto right_tree = rhs->GenerateCode(ridx);

  for (auto &op : left_tree) {
    operations.push_back(std::move(op));
  }

  for (auto &op : right_tree) {
    operations.push_back(std::move(op));
  }

  reg = ridx++;
  operations.push_back(
      std::format("{} r{} r{} r{}", m_sop, lhs->GetValue(), rhs->GetValue(), reg));

  return operations;
}
}  // namespace tci
