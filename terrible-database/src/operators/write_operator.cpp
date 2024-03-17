#include "operators/write_operator.h"
#include <iostream>
#include <format>
#include <sstream>

namespace tdb {
  void WriteOperator::WriteTable() {
    auto ncols = tables[0]->GetColumnSize();
    auto nrows = tables[0]->GetRowSize();

    std::cout<<"\n";
    std::cout<<tables[0]->GetTableName()<<"\n";
    std::stringstream ss;
    for(size_t i = 0; i < ncols; ++i) {
      ss<<tables[0]->GetColumnName(i)<<"\t";
    }

    auto col_names = ss.str();
    std::string dash(1.5*col_names.size(), '-');
    std::cout<<dash<<"\n";
    std::cout<<col_names<<"\n";
    std::cout<<dash<<"\n";
    for(size_t i = 0; i < nrows; ++i) {
      for(size_t j = 0; j < ncols; ++j) {
        switch(tables[0]->GetColumnType(j)) {
          case DT_INT:
            std::cout<<tables[0]->GetValue<sINT>(j, i)<<"\t";
            break;
          case DT_DOUBLE:
            std::cout<<tables[0]->GetValue<sDOUBLE>(j, i)<<"\t";
            break;
          case DT_STRING:
            std::cout<<tables[0]->GetValue<sSTRING>(j, i)<<"\t";
            break;
          default:
            break;
        }
      }
      std::cout<<"\n";
    }
    std::cout<<"\n";
  }
}
