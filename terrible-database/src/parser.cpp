#include "parser.h"
#include "tokenizer.h"
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
    if (!csm.check_transition(token, word)) {
      // std::cout<<"token: "<<token<<", word: "<<word<<std::endl;
      throw std::runtime_error("Failed to parse create query at: " + word);
    }
  }

  if (csm.EOP()) {
    std::cout << "Succefully Parsed Create Query" << std::endl;
    std::cout << "Table Name: " << csm.table_name << std::endl;
    for (size_t i = 0; i < csm.col_types.size(); ++i) {
      std::cout << "Col Name: " << csm.col_names[i]
                << ", Col Type: " << csm.col_types[i] << std::endl;
    }
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
    default:
      throw std::runtime_error("invalid query");
    }
  }

  return operators;
}
} // namespace tdb
