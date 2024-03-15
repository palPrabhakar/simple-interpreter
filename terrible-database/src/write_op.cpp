#include "write_op.h"
#include <iostream>
#include <format>
#include <sstream>

namespace tdb {
  void WriteOp::WriteTable(Table table) {
    // std::cout<<"WriteOp::WriteTable"<<std::endl;
    auto ncols = table.GetColumnSize();
    auto nrows = table.GetRowSize();

    std::cout<<table.GetTableName()<<"\n";
    std::stringstream ss;
    for(size_t i = 0; i < ncols; ++i) {
      ss<<table.GetColumnName(i)<<"\t";
    }

    auto col_names = ss.str();
    std::string dash(1.5*col_names.size(), '-');
    std::cout<<dash<<"\n";
    std::cout<<col_names<<"\n";
    std::cout<<dash<<"\n";
    for(size_t i = 0; i < nrows; ++i) {
      for(size_t j = 0; j < ncols; ++j) {
        switch(table.GetColumnType(j)) {
          case DT_INT:
            std::cout<<table.GetValue<sINT>(j, i)<<"\t";
            break;
          case DT_DOUBLE:
            std::cout<<table.GetValue<sDOUBLE>(j, i)<<"\t";
            break;
          case DT_STRING:
            std::cout<<table.GetValue<sSTRING>(j, i)<<"\t";
            break;
          default:
            break;
        }
      }
      std::cout<<"\n";
    }
  }
}
