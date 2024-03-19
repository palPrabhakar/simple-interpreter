#include "operators/operator.h"
#include "operators/write_operator.h"
#include "parser.h"
#include "table.h"

int main() {
    auto operators = tdb::ParseInputQuery("select * from jl_table join jr_table on ( jl_table.gid == jr_table.id )");

    assert(operators.size() == 5);

    // tdb::Table_Vec vec;
    // operators[0]->AddData(std::move(vec));
    // operators[0]->Execute();
    // vec = std::move(operators[0]->GetData());

    // tdb::StdOutWriter op;
    // op.AddData(std::move(vec));
    // op.Execute();

    // operators[1]->AddData(std::move(vec));
    // operators[1]->Execute();
    // vec = std::move(operators[1]->GetData());

    // tdb::Table_Vec v2;
    // v2.emplace_back(std::move(vec[1]));

    // tdb::StdOutWriter op;
    // op.AddData(std::move(v2));
    // op.Execute();

    tdb::Table_Vec vec;
    for (auto &op : operators) {
      op->AddData(std::move(vec));
      op->Execute();
      vec = std::move(op->GetData());
    }

    return 0;
}
