#pragma once

#include "operator.h"
#include <iostream>
#include <stdexcept>

namespace tdb {

template <class A> class FilterOperator : public BinaryOperator {
public:
  FilterOperator(std::string name, std::string val)
      : col_name(name), value(val) {}

  void Execute() {
    auto col_idx = tables[0]->GetColumnIndex(col_name);
    auto type = tables[0]->GetColumnType(col_idx);
    FilterColumns(tables[0]);
  }

  std::vector<size_t> GetArgResults(Table_Ptr &ptr) {
    auto col_idx = ptr->GetColumnIndex(col_name);
    type = ptr->GetColumnType(col_idx);
    FilterColumns(ptr);
    return std::move(arg_results);
  }

protected:
  std::string col_name;
  std::string value;
  Data_Type type;

  void FilterColumns(Table_Ptr &ptr) {
    try {
      switch (type) {
      case DT_INT:
        static_cast<A *>(this)->FilterInt64(ptr, std::stoi(value));
        break;
      case DT_DOUBLE:
        static_cast<A *>(this)->FilterDouble(ptr, std::stod(value));
        break;
      case DT_STRING:
        static_cast<A *>(this)->FilterStrings(ptr, value);
        break;
      default:
        std::cout << "type: " << type << std::endl;
        throw std::runtime_error(
            "Filter Op: Invalid column type. Column name: " + col_name + "\n");
      }
    } catch (std::invalid_argument const &ex) {
      std::cout << "Filter Operator: Value and Column type mismatch\n";
    }
  }
};

class EqualityFilter : public FilterOperator<EqualityFilter> {
public:
  EqualityFilter(std::string col_name, std::string col_val)
      : FilterOperator<EqualityFilter>(col_name, col_val) {}

  void FilterStrings(Table_Ptr &ptr, std::string val) {
    auto col_idx = ptr->GetColumnIndex(this->col_name);
    auto *cols = ptr->GetColumn(col_idx);
    auto row_size = ptr->GetRowSize();
    StringColumn *scols = static_cast<StringColumn *>(cols);
    for (auto i = 0; i < row_size; ++i) {
      if ((*scols)[i] == val)
        this->arg_results.push_back(i);
    }
  }

  void FilterInt64(Table_Ptr &ptr, int64_t val) {
    auto col_idx = ptr->GetColumnIndex(this->col_name);
    auto *cols = ptr->GetColumn(col_idx);
    auto row_size = ptr->GetRowSize();
    Int64Column *icols = static_cast<Int64Column *>(cols);
    for (auto i = 0; i < row_size; ++i) {
      if ((*icols)[i] == val)
        this->arg_results.push_back(i);
    }
  }

  void FilterDouble(Table_Ptr &ptr, double val) {
    auto col_idx = ptr->GetColumnIndex(this->col_name);
    auto *cols = ptr->GetColumn(col_idx);
    auto row_size = ptr->GetRowSize();
    DoubleColumn *icols = static_cast<DoubleColumn *>(cols);
    for (auto i = 0; i < row_size; ++i) {
      if ((*icols)[i] == val)
        this->arg_results.push_back(i);
    }
  }
};

// class LessThanFilter : public FilterOperator<LessThanFilter> {
// private:
//   void FilterColumns(Table_Ptr &ptr);
//   void FilterStrings(Table_Ptr &ptr);
//   void FilterInt64(Table_Ptr &ptr);
//   void FilterDouble(Table_Ptr &ptr);

//   bool negate;
// };

// class LessEqualFilter : public FilterOperator<LessEqualFilter> {
//   private:
//   void FilterColumns(Table_Ptr &ptr);
//   void FilterStrings(Table_Ptr &ptr);
//   void FilterInt64(Table_Ptr &ptr);
//   void FilterDouble(Table_Ptr &ptr);

// };

// class GreaterEqualFilter : public FilterOperator<GreaterEqualFilter> {
//   private:
//   void FilterColumns(Table_Ptr &ptr);
//   void FilterStrings(Table_Ptr &ptr);
//   void FilterInt64(Table_Ptr &ptr);
//   void FilterDouble(Table_Ptr &ptr);

// };

} // namespace tdb
