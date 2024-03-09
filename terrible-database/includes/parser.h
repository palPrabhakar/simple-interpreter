#ifndef PARSER_H
#define PARSER_H

#include "tokenizer.h"
#include <string>
#include <vector>

namespace tdb {

std::vector<std::string> ParseInputQuery(std::string input_query);

} // namespace tdb

#endif
