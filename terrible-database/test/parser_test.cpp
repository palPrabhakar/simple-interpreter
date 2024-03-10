#include "tokenizer.h"
#include "gtest/gtest.h"
#include <exception>
#include <iostream>
#include <parser.h>
#include <stdexcept>

TEST(ParserTest, CreateQueries) {
  EXPECT_NO_THROW(tdb::ParseInputQuery(
      "Create random_table with col1 int col2 double col3 string"));

  EXPECT_NO_THROW(tdb::ParseInputQuery("Create some_table with col1 int\n"));

  EXPECT_THROW(
      {
        try {
          tdb::ParseInputQuery("Create some_table with col1 int.");
        } catch (std::exception &err) {
          EXPECT_TRUE(std::string(err.what()).find("Expected column type.") !=
                      std::string::npos);
          throw;
        }
      },
      std::runtime_error);

  EXPECT_THROW(
      {
        try {
          tdb::ParseInputQuery("Create random_table with col1 int col2 far");
        } catch (std::exception &err) {
          EXPECT_TRUE(std::string(err.what()).find("Expected column type.") !=
                      std::string::npos);
          throw;
        }
      },
      std::runtime_error);

  EXPECT_THROW(
      {
        try {
          tdb::ParseInputQuery("Create random_table with col1 int double");
        } catch (std::exception &err) {
          EXPECT_TRUE(std::string(err.what()).find("Expected column name.") !=
                      std::string::npos);
          EXPECT_TRUE(
              std::string(err.what()).find("Expected endline character.") !=
              std::string::npos);
          throw;
        }
      },
      std::runtime_error);

  EXPECT_THROW(
      {
        try {
          tdb::ParseInputQuery("Create with col1 int col2 double");
        } catch (std::exception &err) {
          EXPECT_TRUE(std::string(err.what()).find("Expected table name.") !=
                      std::string::npos);
          throw;
        }
      },
      std::runtime_error);

  EXPECT_THROW(
      {
        try {
          tdb::ParseInputQuery("Create random_table col1 int col2 double");
        } catch (std::exception &err) {
          EXPECT_TRUE(std::string(err.what()).find("Expected keyword with.") !=
                      std::string::npos);
          throw;
        }
      },
      std::runtime_error);
}

TEST(ParserTest, InsertQueries) {
  EXPECT_NO_THROW(
      tdb::ParseInputQuery("Insert random_table values 100 250 lol"));

  EXPECT_NO_THROW(
      tdb::ParseInputQuery("Insert some_table values 250.00 very-lol\n"));

  EXPECT_NO_THROW(
      tdb::ParseInputQuery("Insert some_table values lol1 lol...lol"));

  EXPECT_THROW(
      {
        try {
          tdb::ParseInputQuery("Insert values col1 int col2 far");
        } catch (std::exception &err) {
          EXPECT_TRUE(std::string(err.what()).find("Expected table name.") !=
                      std::string::npos);
          throw;
        }
      },
      std::runtime_error);

  EXPECT_THROW(
      {
        try {
          tdb::ParseInputQuery("Insert random_table values col1 int");
        } catch (std::exception &err) {
          EXPECT_TRUE(std::string(err.what()).find("Expected column values.") !=
                      std::string::npos);
          EXPECT_TRUE(
              std::string(err.what()).find("Expected endline character.") !=
              std::string::npos);
          throw;
        }
      },
      std::runtime_error);

  EXPECT_THROW(
      {
        try {
          tdb::ParseInputQuery("Insert random_table with col1 int col2 double");
        } catch (std::exception &err) {
          EXPECT_TRUE(
              std::string(err.what()).find("Expected keyword values.") !=
              std::string::npos);
          throw;
        }
      },
      std::runtime_error);
}

TEST(ParserTest, ExprParsing) {
  auto tokens = tdb::ReadInputQuery("( col1 == 100 )");
  size_t index = 0;
  EXPECT_EQ(tdb::ParseExpression(tokens, index), "col1 == 100");

  tokens = tdb::ReadInputQuery("( col1 > 100 )");
  index = 0;
  EXPECT_EQ(tdb::ParseExpression(tokens, index), "col1 > 100");

  tokens = tdb::ReadInputQuery(" col1 > 100 )");
  index = 0;
  EXPECT_THROW(
      {
        try {
          tdb::ParseExpression(tokens, index);
        } catch (std::exception &err) {
          EXPECT_TRUE(
              std::string(err.what()).find("Expected left parenthesis.") !=
              std::string::npos);
          throw;
        }
      },
      std::runtime_error);

  tokens = tdb::ReadInputQuery(" (  > 100 )");
  index = 0;
  EXPECT_THROW(
      {
        try {
          tdb::ParseExpression(tokens, index);
        } catch (std::exception &err) {
          EXPECT_TRUE(std::string(err.what()).find("Expected column name.") !=
                      std::string::npos);
          throw;
        }
      },
      std::runtime_error);

  tokens = tdb::ReadInputQuery(" ( col1 100 )");
  index = 0;
  EXPECT_THROW(
      {
        try {
          tdb::ParseExpression(tokens, index);
        } catch (std::exception &err) {
          EXPECT_TRUE(
              std::string(err.what()).find("Expected a binary operator.") !=
              std::string::npos);
          throw;
        }
      },
      std::runtime_error);

  tokens = tdb::ReadInputQuery(" ( col1 <  )");
  index = 0;
  EXPECT_THROW(
      {
        try {
          tdb::ParseExpression(tokens, index);
        } catch (std::exception &err) {
          EXPECT_TRUE(std::string(err.what()).find("Expected column value.") !=
                      std::string::npos);
          throw;
        }
      },
      std::runtime_error);

  tokens = tdb::ReadInputQuery(" ( col1 < 100");
  index = 0;
  EXPECT_THROW(
      {
        try {
          tdb::ParseExpression(tokens, index);
        } catch (std::exception &err) {
          EXPECT_TRUE(
              std::string(err.what()).find("Expected right parenthesis.") !=
              std::string::npos);
          throw;
        }
      },
      std::runtime_error);
}

TEST(ParserTest, WhereClause) {
  auto operations = tdb::ParseInputQuery("where ( col1 == 100 )");

  EXPECT_EQ(operations[0], "col1 == 100");

  operations =
      tdb::ParseInputQuery("where ( ( col1 == 100 ) and ( col2 == 50 ) )");

  EXPECT_EQ(operations[0], "col1 == 100");
  EXPECT_EQ(operations[1], "col2 == 50");
  EXPECT_EQ(operations[2], "and");

  operations = tdb::ParseInputQuery(
      "where ( ( ( col1 == 100 ) and ( col2 == lol ) ) or ( col3 != 50 ) )");

  EXPECT_EQ(operations[0], "col1 == 100");
  EXPECT_EQ(operations[1], "col2 == lol");
  EXPECT_EQ(operations[2], "and");
  EXPECT_EQ(operations[3], "col3 != 50");
  EXPECT_EQ(operations[4], "or");

  operations =
      tdb::ParseInputQuery("where ( ( ( col1 == 100 ) and ( col2 == lol ) ) or "
                           "( ( col3 != 50 ) and ( col4 > 11 ) ) )");

  EXPECT_EQ(operations[0], "col1 == 100");
  EXPECT_EQ(operations[1], "col2 == lol");
  EXPECT_EQ(operations[2], "and");
  EXPECT_EQ(operations[3], "col3 != 50");
  EXPECT_EQ(operations[4], "col4 > 11");
  EXPECT_EQ(operations[5], "and");
  EXPECT_EQ(operations[6], "or");

  EXPECT_THROW(tdb::ParseInputQuery("where ( ( ( col1 == 100 ) ) )"),
               std::runtime_error);

  EXPECT_THROW(
      {
        try {
          auto op = tdb::ParseInputQuery("where ( ( col1 == 100 ) and )");
        } catch (std::exception &err) {
          EXPECT_TRUE(
              std::string(err.what()).find("Expected left parenthesis.") !=
              std::string::npos);
          throw;
        }
      },
      std::runtime_error);

  EXPECT_THROW(
      {
        try {
          auto op = tdb::ParseInputQuery("where ( ( col1 == 100 ) and ( ) )");
        } catch (std::exception &err) {
          EXPECT_TRUE(
              std::string(err.what()).find("Expected left parenthesis.") !=
              std::string::npos);
          throw;
        }
      },
      std::runtime_error);

  EXPECT_THROW(
      {
        try {
          auto op =
              tdb::ParseInputQuery("where ( ( col1 == 100 ) and ( col2 ) )");
        } catch (std::exception &err) {
          EXPECT_TRUE(
              std::string(err.what()).find("Expected a binary operator.") !=
              std::string::npos);
          throw;
        }
      },
      std::runtime_error);

  EXPECT_THROW(
      {
        try {
          operations = tdb::ParseInputQuery(
              "where ( ( ( col1 == 100 ) and ( col2 == lol ) ) or "
              "( ( col3 != 50 ) and ( col4 > 11 ) )");

        } catch (std::exception &err) {
          EXPECT_TRUE(
              std::string(err.what()).find("Expected right parenthesis.") !=
              std::string::npos);
          throw;
        }
      },
      std::runtime_error);
}
