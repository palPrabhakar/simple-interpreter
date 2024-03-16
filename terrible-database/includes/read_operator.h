#pragma once

#include "columns.h"
#include "data_types.h"
#include <algorithm>
#include <json/value.h>
#include <memory>
#include <operator.h>
#include <string>
#include <vector>

namespace tdb {
class ReadOperator : public Operator {
public:
  ReadOperator(std::string file_path) : file_name(file_path) {}

  void AddData(Table_Vec tables) {}

  void Execute() { ReadTable(); }

  Table_Vec GetData() { return std::move(tables); }

private:
  std::string file_name;
  Table_Vec tables;

  void ReadTable();

  std::unique_ptr<BaseColumn> GetColumn(const Json::Value &data, Data_Type type,
                                        const size_t size);
  template <typename T>
  std::unique_ptr<BaseColumn> GetColumn(const Json::Value &data,
                                        const size_t size);
};
} // namespace tdb
