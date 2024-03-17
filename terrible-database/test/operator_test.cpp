#include "operators/filter_operator.h"
#include "operators/project_operator.h"
#include "operators/read_operator.h"
#include "operators/set_operator.h"
#include "operators/write_operator.h"
#include "gtest/gtest.h"

TEST(OperatorTest, ReadOperator) {
  tdb::ReadOperator read_op("/home/pal/workspace/terrible-softwares/"
                            "terrible-database/tables/simple_table.json");
  read_op.Execute();

  auto un_op =
      tdb::UnionOperator(std::make_unique<tdb::EqualityFilter>("age", "44"),
                         std::make_unique<tdb::EqualityFilter>("name", "John"));
  un_op.AddData(read_op.GetData());
  un_op.Execute();

  // tdb::EqualityFilter eq_op("age", "44");
  // eq_op.AddData(read_op.GetData());
  // eq_op.Execute();

  tdb::ProjectOperator proj_op({"age"});
  proj_op.AddData(un_op.GetData());
  proj_op.Execute();

  auto write_op = tdb::WriteOperator();
  write_op.AddData(proj_op.GetData());
  write_op.Execute();
  // EXPECT_NO_THROW(op.ReadTable());
}
