#pragma once

#include "columns.h"
#include "data_types.h"
#include <algorithm>
#include <json/value.h>
#include <memory>
#include <string>
#include <table.h>

namespace tdb {
class ReadOperator {
public:
  ReadOperator(std::string file_path) : file_name(file_path) {}

  Table ReadTable();

private:
  std::string file_name;

  std::unique_ptr<BaseColumn> GetColumn(const Json::Value &data, Data_Type type,
                                        const size_t size);

  template <typename T>
  std::unique_ptr<BaseColumn> GetColumn(const Json::Value &data,
                                        const size_t size);
};
} // namespace tdb
