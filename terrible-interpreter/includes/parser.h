#pragma once

#include <memory>
#include <vector>

#include "expr.h"
#include "statement.h"
#include "tokenizer.h"

namespace tci {
void Parse(Tokenizer &tokenizer);

std::unique_ptr<StatementAST> ParseStatement(Tokenizer &tokenizer);

std::unique_ptr<StatementAST> ParseDeclaration(Tokenizer &tokenizer);

std::unique_ptr<ExprAST> ParseExpression(Tokenizer &tokenizer,
                                      std::unique_ptr<ExprAST> expr);

std::unique_ptr<ExprAST> ParseExpression(Tokenizer &tokenizerm);

void ParseFunction(Tokenizer &tokenizer);

void ParseFunctionBody(Tokenizer &tokenizer);

std::vector<std::string> ParseArgumentList(
    Tokenizer &tokenizer);

}  // namespace tci
