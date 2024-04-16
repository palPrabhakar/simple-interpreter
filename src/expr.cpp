#include "expr.h"

#include <iterator>
#include <string>

#include "instructions.h"

namespace tci {

std::vector<Instruction> ValueAST::GenerateCode(uint &ridx) {
  std::vector<Instruction> instructions;
  reg = ridx++;
  instructions.emplace_back(InsCode::loadi, std::stod(m_val),
                            static_cast<int>(reg));
  return instructions;
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

std::vector<Instruction> OpAST::GenerateCode(uint &ridx) {
  auto left_tree = lhs->GenerateCode(ridx);
  auto right_tree = rhs->GenerateCode(ridx);

  std::vector<Instruction> operations;
  operations.reserve(left_tree.size() + right_tree.size() + 1);

  std::copy(left_tree.begin(), left_tree.end(), std::back_inserter(operations));
  std::copy(right_tree.begin(), right_tree.end(),
            std::back_inserter(operations));

  reg = ridx++;
  operations.emplace_back(m_code, static_cast<int>(lhs->GetValue()),
                          static_cast<int>(rhs->GetValue()),
                          static_cast<int>(reg));

  return operations;
}

std::vector<Instruction> FunctionCallAST::GenerateCode(uint &ridx) {
  std::vector<Instruction> operations;

  for (auto &expr : m_args) {
    auto ops = expr->GenerateCode(ridx);
    std::copy(ops.begin(), ops.end(), std::back_inserter(operations));
  }

  operations.emplace_back(InsCode::call, m_name);

  for (size_t i = 0; i < m_args.size(); ++i) {
    auto &expr = m_args[i];
    auto name = m_argnames[i];
    operations.emplace_back(InsCode::store, static_cast<int>(expr->GetValue()),
                            name);
  }

  std::copy(m_body.begin(), m_body.end(), std::back_inserter(operations));

  return operations;
}

}  // namespace tci
