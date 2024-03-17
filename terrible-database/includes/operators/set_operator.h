#pragma once

#include "operator.h"
#include <algorithm>
#include <iterator>

#include <iostream>

namespace tdb {
class UnionOperator : public BinaryOperator {
public:
  UnionOperator(std::unique_ptr<BinaryOperator> &&lhs,
                std::unique_ptr<BinaryOperator> &&rhs)
      : lhs(std::move(lhs)), rhs(std::move(rhs)) {}

  void Execute() { DoUnion(tables[0]); }

  std::vector<size_t> GetArgResults(Table_Ptr &ptr) {
    DoUnion(ptr);
    return std::move(arg_results);
  }

private:
  std::unique_ptr<BinaryOperator> lhs;
  std::unique_ptr<BinaryOperator> rhs;

  void DoUnion(Table_Ptr &ptr) {
    auto left = lhs->GetArgResults(tables[0]);
    auto right = rhs->GetArgResults(tables[0]);

    std::set_union(left.begin(), left.end(), right.begin(), right.end(),
                   std::back_inserter(arg_results));
  }
};

class IntersectionOperator : public BinaryOperator {
public:
  IntersectionOperator(std::unique_ptr<BinaryOperator> &&lhs,
                       std::unique_ptr<BinaryOperator> &&rhs)
      : lhs(std::move(lhs)), rhs(std::move(rhs)) {}

  void Execute() { DoIntersection(tables[0]); }

  std::vector<size_t> GetArgResults(Table_Ptr &ptr) {
    DoIntersection(ptr);
    return std::move(arg_results);
  }

private:
  Table_Vec tables;
  std::vector<size_t> arg_results;

  std::unique_ptr<BinaryOperator> lhs;
  std::unique_ptr<BinaryOperator> rhs;

  void DoIntersection(Table_Ptr &ptr) {
    auto left = lhs->GetArgResults(tables[0]);
    auto right = rhs->GetArgResults(tables[0]);

    std::set_intersection(left.begin(), left.end(), right.begin(), right.end(),
                          std::back_inserter(arg_results));
  }
};

} // namespace tdb
