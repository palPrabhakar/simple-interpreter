#include <iostream>
#include <parser.h>

int main() {
    std::cout<<"Parser Incoming\n";

    // auto op_pipeline = tdb::ParseInputQuery("Select * from random_table where col1 > 100 and col2 < 50");

    auto op_pipeline = tdb::ParseInputQuery("Create random_table with col1 int col2 double col3 string");
    auto op_pipeline2 = tdb::ParseInputQuery("Create random_table with col1 int col2 far");

    return 0;
}
