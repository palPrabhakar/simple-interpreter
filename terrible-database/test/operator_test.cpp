#include "operators/project_operator.h"
#include "operators/read_operator.h"
#include "operators/write_operator.h"
#include "operators/filter_operator.h"
#include "gtest/gtest.h"

TEST(OperatorTest, ReadOperator) {
  tdb::ReadOperator read_op("/home/pal/workspace/terrible-softwares/terrible-database/tables/simple_table.json");
  read_op.Execute();

  tdb::EqualityFilter eq_op("age", "44");
  eq_op.AddData(read_op.GetData());
  eq_op.Execute();

  tdb::ProjectOperator proj_op({"age"});
  proj_op.AddData(eq_op.GetData());
  proj_op.Execute();

  auto write_op = tdb::WriteOperator();
  write_op.AddData(proj_op.GetData());
  write_op.Execute();
  // EXPECT_NO_THROW(op.ReadTable());
}
