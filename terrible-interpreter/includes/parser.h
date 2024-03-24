#pragma once

#include <memory>

#include "expr.h"
#include "statement.h"
#include "tokenizer.h"

namespace tci {
void Parse(Tokenizer &tokenizer);
void ParseStatement(Tokenizer &tokenizer);

std::unique_ptr<Expr<int>> ParseExpression(Tokenizer &tokenizer,
                                         std::unique_ptr<OpExpr<int, int>> expr);

std::unique_ptr<Expr<int>> ParseExpression(Tokenizer &tokenizer);
}  // namespace tci
