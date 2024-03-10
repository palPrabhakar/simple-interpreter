#include "parser.h"
#include "create_state_machine.h"
#include "expr_state_machine.h"
#include "insert_state_machine.h"
#include "tokenizer.h"
#include <cassert>
#include <iostream>
#include <stdexcept>

namespace tdb {

std::vector<std::string> ParseWhereClause(Token_Vector &tokens,
                                          std::size_t &index) {
  auto operations = ParseLogicalOP(tokens, ++index);

  assert(index < tokens.size() && "ParseWhereClause: Index out of range");

  if (tokens[index].first != END) {
    std::runtime_error("Failed to parse where clause. Expected end of line "
                       "character. But found " +
                       tokens[index].second);
  }

  return operations;
}

std::string ParseExpression(Token_Vector &tokens, std::size_t &index) {
  ExprStateMachine esm;

  std::string operation;
  Token op;
  std::string str_op;

  while (index < tokens.size()) {
    if (esm.CheckTransition(tokens[index].first, tokens[index].second)) {
      index++;
    } else {
      if (esm.CheckErrorState()) {
        throw std::runtime_error(esm.GetErrorMsg() + " But found " +
                                 tokens[index].second);
      }
      break;
    }
  }

  if (esm.EOP()) {
    operation = esm.column_name + " " + esm.str_op + " " + esm.column_val;
  } else {
    throw std::runtime_error("Failed to parse expression\n");
  }

  return operation;
}

std::vector<std::string> ParseLogicalOP(Token_Vector &tokens,
                                        std::size_t &index) {
  std::vector<std::string> operators;
  std::vector<std::string> left;
  std::vector<std::string> right;
  std::string str_op;
  Token op_token;

  assert(index + 1 < tokens.size() && "ParseLogicalOP: Index out of range");

  if (tokens[index + 1].first == TEXT) {
    // parse expression returns one past right paren
    // does both left and right parsing
    // leaf element found
    operators.emplace_back(ParseExpression(tokens, index));
    return operators;
  }

  if (tokens[index + 1].first == LEFT_PAREN) {
    left = ParseLogicalOP(tokens, ++index);
  } else {
    throw std::runtime_error("Failed to parse where clause. Expected left "
                             "parenthesis. But found " +
                             tokens[index + 1].second);
  }

  assert(index < tokens.size() && "ParseLogicalOP: Index out of range");

  if (tokens[index].first == AND || tokens[index].first == OR) {
    op_token = tokens[index].first;
    str_op = tokens[index].second;
  } else {
    throw std::runtime_error("Failed to parse where clause. Expected AND or OR "
                             "operator. But found " +
                             tokens[index].second);
  }

  assert(index + 1 < tokens.size() && "ParseLogicalOP: Index out of range");

  if (tokens[index + 1].first == LEFT_PAREN) {
    right = ParseLogicalOP(tokens, ++index);
  } else {
    throw std::runtime_error("Failed to parse where clause. Expected left "
                             "parenthesis. But found " +
                             tokens[index + 1].second);
  }

  assert(index < tokens.size() && "ParseLogicalOP: Index out of range");

  if (tokens[index].first == RIGHT_PAREN) {
    operators.insert(operators.end(), left.begin(), left.end());
    operators.insert(operators.end(), right.begin(), right.end());
    operators.push_back(str_op);
    ++index;
  } else {
    throw std::runtime_error(
        "Failed to parse where clause. Expected right parenthesis. But found " +
        tokens[index].second);
  }

  return operators;
}

std::vector<std::string>
ParseCreateQuery(std::vector<std::pair<Token, std::string>> &tokens,
                 std::size_t &index) {
  std::vector<std::string> operators;

  CreateStateMachine csm;

  for (auto [token, word] : tokens) {
    if (!csm.CheckTransition(token, word)) {
      throw std::runtime_error(csm.GetErrorMsg() + " But found " + word);
    }
  }

  if (csm.EOP()) {
    std::cout << "\nSuccefully Parsed Create Query\n";
    std::cout << "Table Name: " << csm.table_name << "\n";
    for (size_t i = 0; i < csm.col_types.size(); ++i) {
      std::cout << "Col Name: " << csm.col_names[i]
                << ", Col Type: " << csm.col_types[i] << "\n";
    }
    std::cout << "\n";
  } else {
    throw std::runtime_error("Failed to parse create query\n");
  }

  return operators;
}

std::vector<std::string>
ParseInsertQuery(std::vector<std::pair<Token, std::string>> &tokens,
                 std::size_t &index) {
  std::vector<std::string> operators;

  InsertStateMachine ism;

  for (auto [token, word] : tokens) {
    if (!ism.CheckTransition(token, word)) {
      throw std::runtime_error(ism.GetErrorMsg() + " But found " + word);
    }
  }

  if (ism.EOP()) {
    std::cout << "\nSuccefully Parsed Insert Query\n";
    std::cout << "Table Name: " << ism.table_name << "\n";
    for (auto val : ism.col_values) {
      std::cout << "Col Value: " << val << "\n";
    }
    std::cout << "\n";
  } else {
    throw std::runtime_error("Failed to parse create query\n");
  }

  return operators;
}

std::vector<std::string> ParseInputQuery(std::string input_query) {
  auto tokens = ReadInputQuery(input_query);

  // for (auto [token, word] : tokens) {
  //   std::cout << "word: " << word << ", token: " << token << std::endl;
  // }

  std::vector<std::string> operators;
  std::size_t index = 0;

  if (tokens.size() != 0) {
    switch (tokens[0].first) {
    case CREATE:
      operators = ParseCreateQuery(tokens, index);
      break;
    case INSERT:
      operators = ParseInsertQuery(tokens, index);
      break;
    case WHERE:
      operators = ParseWhereClause(tokens, index);
      break;
    default:
      throw std::runtime_error("invalid query");
    }
  }

  return operators;
}
} // namespace tdb
