#include "project_operator.h"
#include "read_operator.h"
#include "write_operator.h"
#include "gtest/gtest.h"

TEST(OperatorTest, ReadOperator) {
  tdb::ReadOperator read_op("/home/pal/workspace/terrible-softwares/terrible-database/tables/simple_table.json");
  read_op.Execute();

  tdb::ProjectOperator proj_op({"name"});
  proj_op.AddData(read_op.GetData());
  proj_op.Execute();

  auto write_op = tdb::WriteOperator();
  write_op.AddData(proj_op.GetData());
  write_op.Execute();
  // EXPECT_NO_THROW(op.ReadTable());
}
