#include "read_op.h"
#include "data_types.h"
#include "json/json.h"
#include <cstdint>
#include <fstream>
#include <iostream>
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

Table ReadOperator::ReadTable() {
  Json::Value data;
  std::ifstream ifs;
  ifs.open(file_name);

  Json::CharReaderBuilder builder;
  builder["collectComments"] = false;
  JSONCPP_STRING errs;
  if (!parseFromStream(builder, ifs, &data, &errs)) {
    // std::cout<<errs<<std::endl;
    throw std::runtime_error(errs.c_str());
  }
  std::cout << data << std::endl;

  size_t ncols = data["schema"]["ncols"].asUInt64();
  size_t nrows = data["schema"]["nrows"].asUInt64();

  // std::cout<<ncols<<", "<<nrows<<std::endl;

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

  // std::cout<<"col_names.size(): "<<col_names.size()<<std::endl;

  // std::cout<<"shape: "<<ncols<<", "<<nrows<<std::endl;
  Table table(ncols, nrows, col_types);
  // std::cout<<"shape: "<<table.GetColumnSize()<<", "<<table.GetRowSize()<<std::endl;

  for (size_t i = 0; i < col_names.size(); ++i) {
    const Json::Value col_val = data["data"][col_names[i]];
    // std::cout<<"Calling GetColumn";
    auto ptr = GetColumn(col_val, col_types[i], nrows);
    // if(col_types[i] == DT_INT) {
    //   int64_t val;
    //   ptr->get_value(0, val);
    //   std::cout<<"int val: "<<val<<std::endl;
    // } else {
    //   std::string val;
    //   ptr->get_value(0, val);
    //   std::cout<<"string val: "<<val<<std::endl;
    // }
    table.SetColumn(i, std::move(ptr));
  }

  // std::cout<<"Table Read Complete\n";
  // std::cout<<"row_size: "<<table.GetRowSize()<<std::endl;
  // std::cout<<"col_size: "<<table.GetColumnSize()<<std::endl;
  BaseColumn *col1 = table.GetColumn(0);
  // if(!col1) {
  //   std::cout<<"nullptr"<<std::endl;
  // }
  // std::string name;
  // col1->get_value(0, name);
  // std::cout<<"name: "<<name<<std::endl;
  return table;
}

std::unique_ptr<BaseColumn> ReadOperator::GetColumn(const Json::Value &data,
                                                    const Data_Type type,
                                                    const size_t size) {
  // std::cout<<"GetColumn: "<<static_cast<int>(type)<<std::endl;
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
