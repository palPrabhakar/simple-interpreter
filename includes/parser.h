#pragma once

#include <memory>
#include <vector>

#include "ast.h"
#include "expr.h"
#include "statement.h"
#include "symbol_table.h"
#include "tokenizer.h"
#include "function.h"

namespace sci {
std::unique_ptr<BaseAST> Parse(Tokenizer &tokenizer, SymbolTable &st);

std::unique_ptr<StatementAST> ParseStatement(Tokenizer &tokenizer,
                                             SymbolTable &st);

std::unique_ptr<StatementAST> ParseDeclaration(Tokenizer &tokenizer,
                                               SymbolTable &st);

std::unique_ptr<ExprAST> ParseExpression(Tokenizer &tokenizer,
                                         std::unique_ptr<ExprAST> expr, SymbolTable &st);

std::unique_ptr<ExprAST> ParseExpression(Tokenizer &tokenizer, SymbolTable &st);

std::unique_ptr<IfStatementAST> ParseIfStatement(Tokenizer &tokenizer, SymbolTable &st);

template<Token tk>
std::unique_ptr<LoopCtrlStatementAST<tk>> ParseLoopCtrlStatement(Tokenizer &tokenizer, SymbolTable &st) {
  if (tokenizer.EOP()) {
    throw std::runtime_error("Unexpected end of parsing.\n");
  }
  auto [token, word] = tokenizer.GetNextToken();
  if (token != SColon) {
    throw std::runtime_error(
        std::format("Expected ; but found {} at pos {}.\n", word,
                    tokenizer.GetPos()));
  }
  return std::make_unique<LoopCtrlStatementAST<tk>>();
}

std::unique_ptr<WhileStatementAST> ParseWhileStatement(Tokenizer &tokenzier, SymbolTable &st);

std::shared_ptr<FunctionAST> ParseFunction(Tokenizer &tokenizer, SymbolTable &st);

std::unique_ptr<ReturnStatementAST> ParseReturnStatement(Tokenizer &tokenizer, SymbolTable &st);

std::unique_ptr<FunctionCallAST> ParseFunctionCall(Tokenizer &tokenizer, SymbolTable &st);

std::vector<std::unique_ptr<BaseAST>> ParseConditionalBody(Tokenizer &tokenizer, SymbolTable &st);

std::vector<std::unique_ptr<BaseAST>> ParseFunctionBody(Tokenizer &tokenizer, SymbolTable &st);

std::vector<std::string> ParseArgumentList(Tokenizer &tokenizer, SymbolTable &st);
}  // namespace sci
