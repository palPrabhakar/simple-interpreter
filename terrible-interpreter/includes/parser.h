#pragma once

#include <memory>
#include <vector>

#include "expr.h"
#include "statement.h"
#include "tokenizer.h"

namespace tci {
void Parse(Tokenizer &tokenizer);
void ParseStatement(Tokenizer &tokenizer);

void ParseFunction(Tokenizer &tokenizer);
std::vector<std::pair<Token, std::string>> ParseArgumentList(
    Tokenizer &tokenizer);

std::unique_ptr<Expr> ParseExpression(Tokenizer &tokenizer,
                                      std::unique_ptr<OpExpr> expr);

std::unique_ptr<Expr> ParseExpression(Tokenizer &tokenizer);
}  // namespace tci
