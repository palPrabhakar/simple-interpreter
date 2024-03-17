#include "parser.h"
#include "fsms/create_state_machine.h"
#include "fsms/expr_state_machine.h"
#include "fsms/insert_state_machine.h"
#include "fsms/select_state_machine.h"
#include "fsms/update_state_machine.h"
#include "operators/project_operator.h"
#include "operators/read_operator.h"
#include "operators/set_operator.h"
#include "operators/write_operator.h"
#include "tokenizer.h"
#include <cassert>
#include <format>
#include <iostream>
#include <memory>
#include <stdexcept>

namespace tdb {

Operator_Ptr ParseWhereClause(Token_Vector &tokens, size_t &index) {
  auto operations = ParseLogicalOP(tokens, ++index);

  assert(index < tokens.size() && "ParseWhereClause: Index out of range");

  if (tokens[index].first != END) {
    std::runtime_error("Failed to parse where clause. Expected end of line "
                       "character. But found " +
                       tokens[index].second);
  }

  return operations;
}

BinaryOp_Ptr ParseExpression(Token_Vector &tokens, size_t &index) {
  ExprStateMachine esm;

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

  if (!esm.EOP()) {
    throw std::runtime_error("Failed to parse expression\n");
  }

  return esm.GetOperator();
}

BinaryOp_Ptr ParseLogicalOP(Token_Vector &tokens, size_t &index) {
  BinaryOp_Ptr output;
  BinaryOp_Ptr left;
  BinaryOp_Ptr right;
  std::string str_op;
  Token op_token;

  assert(index + 1 < tokens.size() && "ParseLogicalOP: Index out of range");

  if (tokens[index + 1].first == TEXT) {
    // parse expression returns one past right paren
    // does both left and right parsing
    // leaf element found
    return ParseExpression(tokens, index);
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

  if (tokens[index].first != RIGHT_PAREN) {
    throw std::runtime_error(
        "Failed to parse where clause. Expected right parenthesis. But found " +
        tokens[index].second);
  }
  ++index;

  switch (op_token) {
  case AND:
    output = std::make_unique<IntersectionOperator>(std::move(left),
                                                    std::move(right));
    break;
  case OR:
    output = std::make_unique<UnionOperator>(std::move(left), std::move(right));
    break;
  default:
    break;
  }

  return output;
}

// Operator_Vec ParseUpdateQuery(Token_Vector &tokens, size_t &index) {
//   Operator_Vec operators;

//   UpdateStateMachine usm;
//   usm.RegisterCallBack([&tokens, &index]() {
//     auto operations = ParseWhereClause(tokens, index);
//     --index;
//     return operations;
//   });

//   assert(index < tokens.size() && "ParseUpdateQuery: Index out of range\n");

//   for (; index < tokens.size(); ++index) {
//     if (!usm.CheckTransition(tokens[index].first, tokens[index].second)) {
//       if (usm.CheckErrorState()) {
//         throw std::runtime_error(usm.GetErrorMsg() + " But found " +
//                                  tokens[index].second);
//       }
//       break;
//     }
//   }

//   if (usm.EOP()) {
//     std::cout << "\nSuccefully Parsed Update Query\n";
//     std::cout << "Table Name: " << usm.table_name << "\n";
//     for (size_t i = 0; i < usm.col_names.size(); ++i) {
//       std::cout << "Col Name: " << usm.col_names[i]
//                 << ", Col Value: " << usm.col_values[i] << "\n";
//     }
//     std::cout << "Where clause op: \n";
//     for (auto op : usm.where_clause) {
//       std::cout << op << "\n";
//     }
//     std::cout << "\n";
//   } else {
//     throw std::runtime_error("Failed to parse update query\n");
//   }

//   return operators;
// }

Operator_Vec ParseSelectQuery(Token_Vector &tokens, size_t &index) {
  Operator_Vec operators;

  SelectStateMachine ssm;
  ssm.RegisterCallBack([&tokens, &index]() {
    auto operations = ParseWhereClause(tokens, index);
    --index;
    return operations;
  });

  assert(index < tokens.size() && "ParseSelectQuery: Index out of range\n");

  for (; index < tokens.size(); ++index) {
    if (!ssm.CheckTransition(tokens[index].first, tokens[index].second)) {
      if (ssm.CheckErrorState()) {
        throw std::runtime_error(ssm.GetErrorMsg() + " But found " +
                                 tokens[index].second);
      }
      break;
    }
  }

  if (!ssm.EOP()) {
    throw std::runtime_error("Failed to parse select query\n");
  }

  auto file_path = std::format(
      "/home/pal/workspace/terrible-softwares/terrible-database/tables/{}.json",
      ssm.table_name);
  // Add Table Read Operator
  operators.emplace_back(std::make_unique<ReadOperator>(file_path));

  // If where clause add Binary Operator
  if (ssm.where_op) {
    operators.emplace_back(std::move(ssm.where_op));
  }

  // Add Project Operator
  operators.emplace_back(std::make_unique<ProjectOperator>(ssm.col_names));

  // Add Write Operator
  operators.emplace_back(std::make_unique<WriteOperator>());

  return operators;
}

// Operator_Vec ParseCreateQuery(Token_Vector &tokens, size_t &index) {
//   Operator_Vec operators;

//   CreateStateMachine csm;

//   for (auto [token, word] : tokens) {
//     if (!csm.CheckTransition(token, word)) {
//       throw std::runtime_error(csm.GetErrorMsg() + " But found " + word);
//     }
//   }

//   if (csm.EOP()) {
//     std::cout << "\nSuccefully Parsed Create Query\n";
//     std::cout << "Table Name: " << csm.table_name << "\n";
//     for (size_t i = 0; i < csm.col_types.size(); ++i) {
//       std::cout << "Col Name: " << csm.col_names[i]
//                 << ", Col Type: " << csm.col_types[i] << "\n";
//     }
//     std::cout << "\n";
//   } else {
//     throw std::runtime_error("Failed to parse create query\n");
//   }

//   return operators;
// }

// Operator_Vec ParseInsertQuery(Token_Vector &tokens, size_t &index) {
//   Operator_Vec operators;

//   InsertStateMachine ism;

//   for (auto [token, word] : tokens) {
//     if (!ism.CheckTransition(token, word)) {
//       throw std::runtime_error(ism.GetErrorMsg() + " But found " + word);
//     }
//   }

//   if (ism.EOP()) {
//     std::cout << "\nSuccefully Parsed Insert Query\n";
//     std::cout << "Table Name: " << ism.table_name << "\n";
//     for (auto val : ism.col_values) {
//       std::cout << "Col Value: " << val << "\n";
//     }
//     std::cout << "\n";
//   } else {
//     throw std::runtime_error("Failed to parse create query\n");
//   }

//   return operators;
// }

Operator_Vec ParseInputQuery(std::string input_query) {
  auto tokens = ReadInputQuery(input_query);

  // for (auto [token, word] : tokens) {
  //   std::cout << "word: " << word << ", token: " << token << std::endl;
  // }

  Operator_Vec operators;
  std::size_t index = 0;

  if (tokens.size() != 0) {
    switch (tokens[0].first) {
    case CREATE:
      // operators = ParseCreateQuery(tokens, index);
      break;
    case INSERT:
      // operators = ParseInsertQuery(tokens, index);
      break;
    case UPDATE:
      // operators = ParseUpdateQuery(tokens, index);
      break;
    case SELECT:
      return ParseSelectQuery(tokens, index);
    default:
      throw std::runtime_error("invalid query");
    }
  }

  return operators;
}
} // namespace tdb
