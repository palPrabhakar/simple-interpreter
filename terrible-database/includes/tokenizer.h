#ifndef TOKENS_H
#define TOKENS_H

#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace tdb {
// clang-format off
enum Token {
    TEXT,
    CREATE,
    INSERT,
    DELETE,
    UPDATE,
    SELECT,
    WHERE,
    FROM,
    OP_EQ, // ==
    OP_GT, // >
    OP_LT, // <
    OP_EQGT, // >=
    OP_EQLT, // <=
    AND,
    OR,
    TYPE_INT,
    TYPE_DOUBLE,
    TYPE_STRING,
    VALUES,
    WITH,
    END
};

const std::unordered_map<std::string, Token> token_lookup = {
    { "create", CREATE },
    { "insert", INSERT },
    { "update", UPDATE },
    { "delete", DELETE },
    { "select", SELECT },
    { "where", WHERE },
    { "from", FROM },
    { "==", OP_EQ },
    { ">", OP_GT },
    { "<", OP_LT },
    { ">=", OP_EQGT },
    { "<=", OP_EQLT },
    { "and", AND },
    { "or", OR },
    { "int", TYPE_INT },
    { "double", TYPE_DOUBLE},
    { "string", TYPE_STRING},
    { "values", VALUES },
    { "with", WITH }
};
// clang-format on

std::vector<std::pair<Token, std::string>>
ReadInputQuery(std::string input_query);
} // namespace tdb

#endif
