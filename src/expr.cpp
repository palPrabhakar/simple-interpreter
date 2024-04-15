#include "expr.h"

#include <iterator>
#include <string>

#include "instructions.h"

namespace tci {

std::vector<std::string> ValueAST::GenerateCodeStr(uint &ridx) {
  reg = ridx++;
  return {std::format("loadi {} r{}", m_val, reg)};
}

std::vector<Instruction> ValueAST::GenerateCode(uint &ridx) {
  std::vector<Instruction> instructions;
  reg = ridx++;
  instructions.emplace_back(InsCode::loadi, std::stod(m_val),
                            static_cast<int>(reg));
  return instructions;
}

std::vector<std::string> VarAST::GenerateCodeStr(uint &ridx) {
  reg = ridx++;
  return {std::format("load m@{} r{}", m_name, reg)};
}

std::vector<Instruction> VarAST::GenerateCode(uint &ridx) {
  std::vector<Instruction> instructions;
  reg = ridx++;
  instructions.emplace_back(InsCode::load, m_name, static_cast<int>(reg));
  return instructions;
}

OpAST::OpAST(Token op, std::string sop) : m_op(op), m_sop(sop) {
  switch (m_op) {
    case Add:
      m_code = InsCode::add;
      m_sop = "add";
      m_precedence = 20;
      break;
    case Sub:
      m_code = InsCode::sub;
      m_sop = "sub";
      m_precedence = 20;
      break;
    case Mul:
      m_code = InsCode::mul;
      m_sop = "mul";
      m_precedence = 40;
      break;
    case Div:
      m_code = InsCode::div;
      m_sop = "div";
      m_precedence = 40;
      break;
    case Less:
      m_code = InsCode::lt;
      m_sop = "lt";
      m_precedence = 0;
      break;
    case More:
      m_code = InsCode::gt;
      m_sop = "gt";
      m_precedence = 0;
      break;
    case LessEq:
      m_code = InsCode::lteq;
      m_sop = "lteq";
      m_precedence = 0;
      break;
    case MoreEq:
      m_code = InsCode::gteq;
      m_sop = "gteq";
      m_precedence = 0;
      break;
    default:
      m_precedence = 0;
      break;
  }
}

std::vector<std::string> OpAST::GenerateCodeStr(uint &ridx) {
  std::vector<std::string> operations;
  auto left_tree = lhs->GenerateCodeStr(ridx);
  auto right_tree = rhs->GenerateCodeStr(ridx);

  std::copy(left_tree.begin(), left_tree.end(), std::back_inserter(operations));
  std::copy(right_tree.begin(), right_tree.end(), std::back_inserter(operations));

  reg = ridx++;
  operations.push_back(std::format("{} r{} r{} r{}", m_sop,
                                   static_cast<int>(lhs->GetValue()),
                                   static_cast<int>(rhs->GetValue()), reg));

  return operations;
}

std::vector<Instruction> OpAST::GenerateCode(uint &ridx) {
  auto left_tree = lhs->GenerateCode(ridx);
  auto right_tree = rhs->GenerateCode(ridx);

  std::vector<Instruction> operations;
  operations.reserve(left_tree.size() + right_tree.size() + 1);

  std::copy(left_tree.begin(), left_tree.end(), std::back_inserter(operations));
  std::copy(right_tree.begin(), right_tree.end(), std::back_inserter(operations));

  reg = ridx++;
  operations.emplace_back(m_code, static_cast<int>(lhs->GetValue()),
                          static_cast<int>(rhs->GetValue()),
                          static_cast<int>(reg));

  return operations;
}

std::vector<std::string> FunctionCallAST::GenerateCodeStr(uint &ridx) {
  std::vector<std::string> operations;

  for(auto &expr: m_args) {
    auto ops = expr->GenerateCodeStr(ridx);
    std::copy(ops.begin(), ops.end(), std::back_inserter(operations));
  }

  operations.push_back(std::format("call {}", m_name));

  for(size_t i = 0; i < m_args.size(); ++i) {
    auto &expr = m_args[i];
    auto name = m_argnames[i];
    operations.push_back(std::format("store r{} m@{}", expr->GetValue(), name));
  }

  std::copy(m_sbody.begin(), m_sbody.end(), std::back_inserter(operations));

  return operations;
}

}  // namespace tci
