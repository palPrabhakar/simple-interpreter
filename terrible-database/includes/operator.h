#pragma once

#include "table.h"
#include <memory>
#include <vector>

namespace tdb {
using Table_Vec = std::vector<std::unique_ptr<Table>>;

class Operator {
public:
  virtual void AddData(Table_Vec tables) = 0;
  virtual void Execute() = 0;
  virtual Table_Vec GetData() = 0;
};
} // namespace tdb
