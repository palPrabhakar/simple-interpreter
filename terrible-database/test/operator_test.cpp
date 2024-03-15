#include "read_op.h"
#include "gtest/gtest.h"

TEST(OperatorTest, ReadOperator) {
  tdb::ReadOperator op("/home/pal/workspace/terrible-softwares/terrible-database/tables/simple_table.json");
  auto table = op.ReadTable();
  std::cout<<"col_size: "<<table.GetColumnSize()<<std::endl;
  std::cout<<"row_size: "<<table.GetRowSize()<<std::endl;

  tdb::BaseColumn *col0 = table.GetColumn(0);
  std::string name;
  col0->get_value(0, name);
  std::cout<<name<<std::endl;

  tdb::BaseColumn *col1 = table.GetColumn(1);
  tdb::Column<int64_t> *ptr = static_cast<tdb::Column<int64_t> *>(col1);
  int64_t age;
  ptr->get_value(0, age);
  std::cout<<age<<std::endl;

  // EXPECT_NO_THROW(op.ReadTable());
}
