#pragma once

#include <memory>
#include <vector>
#include "table.h"

namespace tdb {
class WriteOp {
  public:
    void WriteTable(Table table);
};
} // namespace tdb
