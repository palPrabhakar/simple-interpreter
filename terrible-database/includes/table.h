#pragma once

#include "columns.h"
#include "data_types.h"
#include <cassert>
#include <memory>
#include <vector>

namespace tdb {
class Table {
public:
  Table(size_t n_cols, size_t n_rows, std::vector<Data_Type> type_col)
      : n_cols(n_cols), n_rows(n_rows), type_col(type_col), columns(n_cols) {}

  size_t GetColumnSize() { return n_cols; }
  size_t GetRowSize() { return n_rows; }

  void SetRowSize(size_t size) { n_rows = size; }
  void SetColumnSize(size_t size) { n_rows = size; }

  template <typename T> T GetValue(size_t col, size_t row) { return T{}; }

  template <typename T> T SetValue(size_t col, size_t row, T val) {
    return T{};
  }

  BaseColumn *GetColumn(size_t col) { return columns[col].get(); }

  void SetColumn(size_t col, std::unique_ptr<BaseColumn> column) {
    assert(col < columns.size() && "Column index out of bounds");
    columns[col] = std::move(column);
  }

private:
  size_t n_cols;
  size_t n_rows;
  std::vector<Data_Type> type_col;
  std::vector<std::unique_ptr<BaseColumn>> columns;
};
} // namespace tdb
