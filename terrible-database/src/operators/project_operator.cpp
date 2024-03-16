#include "operators/project_operator.h"
#include <memory>

namespace tdb {
  void ProjectOperator::ProcessTable() {
    assert(input_tables.size() == 1 && "Project Operator: Tables size != 1\n");
    auto n_rows = input_tables[0]->GetRowSize();
    auto n_cols = column_names.size();
    std::vector<Data_Type> col_types;
    std::vector<size_t> col_idxs;

    for(auto col_name: column_names) {
      auto idx = input_tables[0]->GetColumnIndex(col_name);
      col_idxs.push_back(idx);
      col_types.push_back(input_tables[0]->GetColumnType(idx));
    }

    output_tables.emplace_back(std::make_unique<Table>(n_cols, n_rows, "temp-project", column_names, col_types));

    auto col_idx = 0;
    for(auto idx: col_idxs) {
      auto ptr = input_tables[0]->ReleaseColumn(idx);
      output_tables[0]->SetColumn(col_idx++, std::move(ptr));
    }
  }
}
