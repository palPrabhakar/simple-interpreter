#pragma once

#include "operator.h"
#include "table.h"
#include <memory>
#include <vector>

namespace tdb {
class WriteOperator : public Operator {
public:
  void AddData(Table_Vec tables) {
    assert(tables.size() == 1 && "WriteOperator: Tables size > 1\n");
    this->tables.emplace_back(std::move(tables[0]));
  }

  void Execute() {
    WriteTable(); }

  Table_Vec GetData() { return std::move(tables); }

private:
  Table_Vec tables;

  void WriteTable();
};
} // namespace tdb
