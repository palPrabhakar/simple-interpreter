#ifndef PARSER_H
#define PARSER_H

#include "tokenizer.h"
#include <string>
#include <vector>

namespace tdb {

std::vector<std::string> ParseInputQuery(std::string input_query);
std::vector<std::string>
ParseCreateQuery(std::vector<std::pair<Token, std::string>> &tokens,
                 std::size_t index);

std::vector<std::string>
ParseInsertQuery(std::vector<std::pair<Token, std::string>> &tokens,
                 std::size_t index);

} // namespace tdb

#endif
