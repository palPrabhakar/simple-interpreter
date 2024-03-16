#include "read_operator.h"
#include "data_types.h"
#include "json/json.h"
#include <cstdint>
#include <fstream>
#include <memory>
#include <stdexcept>

namespace tdb {

template <>
std::unique_ptr<BaseColumn>
ReadOperator::GetColumn<int64_t>(const Json::Value &data, const size_t size) {
  std::vector<int64_t> vec;
  vec.reserve(size);

  for (int i = 0; i < data.size(); ++i) {
    vec.push_back(data[i].asInt64());
  }

  return std::make_unique<Column<int64_t>>(size, std::move(vec));
}

template <>
std::unique_ptr<BaseColumn>
ReadOperator::GetColumn<double>(const Json::Value &data, const size_t size) {
  std::vector<double> vec;
  vec.reserve(size);

  for (int i = 0; i < data.size(); ++i) {
    vec.push_back(data[i].asDouble());
  }

  return std::make_unique<Column<double>>(size, std::move(vec));
}

template <>
std::unique_ptr<BaseColumn>
ReadOperator::GetColumn<std::string>(const Json::Value &data, const size_t size) {
  std::vector<std::string> vec;
  vec.reserve(size);

  for (int i = 0; i < data.size(); ++i) {
    vec.emplace_back(data[i].asString());
  }

  return std::make_unique<Column<std::string>>(size, std::move(vec));
}

void ReadOperator::ReadTable() {
  Json::Value data;
  std::ifstream ifs;
  ifs.open(file_name);

  Json::CharReaderBuilder builder;
  builder["collectComments"] = false;
  JSONCPP_STRING errs;
  if (!parseFromStream(builder, ifs, &data, &errs)) {
    throw std::runtime_error(errs.c_str());
  }

  size_t ncols = data["schema"]["ncols"].asUInt64();
  size_t nrows = data["schema"]["nrows"].asUInt64();
  auto table_name = data["schema"]["name"].asString();

  const Json::Value cols = data["schema"]["columns"];
  const Json::Value types = data["schema"]["types"];

  std::vector<std::string> col_names;
  std::vector<Data_Type> col_types;

  assert(cols.size() == types.size() &&
         "Table: cols and type size does not match\n");

  for (int i = 0; i < cols.size(); ++i) {
    col_names.emplace_back(cols[i].asString());
    col_types.emplace_back(static_cast<Data_Type>(types[i].asInt()));
  }

  assert(tables.size() == 0 && "Vec<Table> not empty\n");

  tables.emplace_back(std::make_unique<Table>(ncols, nrows, table_name, col_names, col_types));
  // Table table(ncols, nrows, table_name, col_names, col_types);

  for (size_t i = 0; i < col_names.size(); ++i) {
    const Json::Value col_val = data["data"][col_names[i]];
    auto ptr = GetColumn(col_val, col_types[i], nrows);
    tables[0]->SetColumn(i, std::move(ptr));
  }
}

std::unique_ptr<BaseColumn> ReadOperator::GetColumn(const Json::Value &data,
                                                    const Data_Type type,
                                                    const size_t size) {
  switch (type) {
  case DT_INT:
    return GetColumn<int64_t>(data, size);
  case DT_DOUBLE:
    return GetColumn<double>(data, size);
  case DT_STRING:
    return GetColumn<std::string>(data, size);
  default:
    throw std::runtime_error("Invalid Type\n");
  }
}
} // namespace tdb
