#pragma once

#include "tokenizer.h"
#include <string>
#include <vector>

namespace tdb {

using Token_Vector = std::vector<std::pair<Token, std::string>>;

std::vector<std::string> ParseInputQuery(std::string input_query);

std::vector<std::string> ParseCreateQuery(Token_Vector &tokens, size_t &index);

std::vector<std::string> ParseInsertQuery(Token_Vector &tokens, size_t &index);

std::vector<std::string> ParseWhereClause(Token_Vector &tokens, size_t &index);

std::string ParseExpression(Token_Vector &tokens, size_t &index);

std::vector<std::string> ParseLogicalOP(Token_Vector &tokens, size_t &index);

std::vector<std::string> ParseUpdateQuery(Token_Vector &tokens, size_t &index);

std::vector<std::string> ParseSelectQuery(Token_Vector &tokens, size_t &index);

} // namespace tdb
