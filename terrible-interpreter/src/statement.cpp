#include "statement.h"

namespace tci {
std::vector<std::string> StatementAST::GenerateCode(uint &ridx) {
  auto operations = m_expr->GenerateCode(ridx);
  operations.push_back(
      std::format("store r{} m@{}", m_expr->GetValue(), m_varName));
  return operations;
}

std::vector<std::string> IfStatementAST::GenerateCode(uint &ridx) {
  std::vector<std::string> operations;

  operations.push_back("label: conditional_block");

  auto expr_tree = m_cexpr->GenerateCode(ridx);
  for (auto &op : expr_tree) {
    operations.push_back(std::move(op));
  }

  std::vector<std::string> fb;
  for (auto &node : false_branch) {
    auto ops = node->GenerateCode(ridx);
    for (auto &op : ops) {
      fb.push_back(std::move(op));
    }
  }

  std::vector<std::string> tb;
  for (auto &node : true_branch) {
    auto ops = node->GenerateCode(ridx);
    for (auto &op : ops) {
      tb.push_back(std::move(op));
    }
  }

  operations.push_back(
      std::format("cjmp r{} p@{}", m_cexpr->GetValue(), fb.size() + 2));

  operations.push_back("label: false_branch");
  for (auto &op : fb) {
    operations.push_back(std::move(op));
  }

  operations.push_back(std::format("jmp p@{}", tb.size() + 1));

  operations.push_back("label: true_branch");
  for (auto &op : tb) {
    operations.push_back(std::move(op));
  }

  return operations;
}

std::vector<std::string> WhileStatementAST::GenerateCode(uint &ridx) {
  std::vector<std::string> operations;

  operations.emplace_back("label: conditional_block");
  auto expr_tree = m_cexpr->GenerateCode(ridx);
  for (auto &op : expr_tree) {
    operations.push_back(std::move(op));
  }


  std::vector<std::string> body;
  for (auto &node : m_body) {
    auto ops = node->GenerateCode(ridx);
    for (auto &op : ops) {
      body.push_back(std::move(op));
    }
  }

  operations.push_back(std::format("cjmp r{} p@{}", m_cexpr->GetValue(), body.size() + 2));
  operations.emplace_back("label: loop_body");

  for (auto &op : body) {
    operations.push_back(std::move(op));
  }

  operations.push_back(std::format("jmp p@-{}", operations.size()));

  return operations;
}

}  // namespace tci
