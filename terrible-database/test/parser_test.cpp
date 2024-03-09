#include "gtest/gtest.h"
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
