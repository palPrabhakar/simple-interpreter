#include "parser.h"
#include "insert_state_machine.h"
#include "tokenizer.h"
#include "create_state_machine.h"
#include <iostream>
#include <stdexcept>

namespace tdb {
std::vector<std::string>
ParseCreateQuery(std::vector<std::pair<Token, std::string>> &tokens,
                 std::size_t index) {
  std::vector<std::string> operators;

  CreateStateMachine csm;

  for (auto [token, word] : tokens) {
    // std::cout<<"token: "<<token<<", word: "<<word<<std::endl;
    if (!csm.CheckTransition(token, word)) {
      // std::cout<<"token: "<<token<<", word: "<<word<<std::endl;
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
    std::cout<<"\n";
  } else {
    throw std::runtime_error("Failed to parse create query\n");
  }

  return operators;
}

std::vector<std::string>
ParseInsertQuery(std::vector<std::pair<Token, std::string>> &tokens,
                 std::size_t index) {
  std::vector<std::string> operators;

  InsertStateMachine ism;

  for (auto [token, word] : tokens) {
    if(!ism.CheckTransition(token, word)) {
      throw std::runtime_error(ism.GetErrorMsg() + " But found " + word);
    }
  }

  if (ism.EOP()) {
    std::cout << "\nSuccefully Parsed Insert Query\n";
    std::cout << "Table Name: " << ism.table_name <<"\n";
    for (auto val: ism.col_values) {
      std::cout << "Col Value: " << val <<"\n";
    }
    std::cout<<"\n";
  } else {
    throw std::runtime_error("Failed to parse create query\n");
  }

  return operators;
}

std::vector<std::string> ParseInputQuery(std::string input_query) {
  auto tokens = ReadInputQuery(input_query);

  // for(auto [token, word] : tokens) {
  //     std::cout<<"word: "<<word<<", token: "<<token<<std::endl;
  // }

  std::vector<std::string> operators;

  if (tokens.size() != 0) {
    switch (tokens[0].first) {
    case CREATE:
      operators = ParseCreateQuery(tokens, 0);
      break;
    case INSERT:
      operators = ParseInsertQuery(tokens, 0);
      break;
    default:
      throw std::runtime_error("invalid query");
    }
  }

  return operators;
}
} // namespace tdb
