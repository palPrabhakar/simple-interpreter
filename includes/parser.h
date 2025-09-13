#pragma once

#include <format>
#include <memory>
#include <vector>

#include "ast.h"
#include "expr.h"
#include "function.h"
#include "statement.h"
#include "symbol_table.h"
#include "tokenizer.h"

namespace sci {

inline void CheckTokenizer(Tokenizer &tokenizer) {
    if (tokenizer.EOP()) {
        throw std::runtime_error("Unexpected end of parsing.\n");
    }
}

std::unique_ptr<BaseAST> Parse(Tokenizer &tokenizer, SymbolTable &st);

std::unique_ptr<StatementAST> ParseStatement(Tokenizer &tokenizer,
                                             SymbolTable &st);

std::unique_ptr<StatementAST> ParseDeclaration(Tokenizer &tokenizer,
                                               SymbolTable &st);

std::unique_ptr<ExprAST> ParseExpression(Tokenizer &tokenizer,
                                         std::unique_ptr<OpAST> expr,
                                         SymbolTable &st);

std::unique_ptr<ExprAST> ParseExpression(Tokenizer &tokenizer, SymbolTable &st);

std::unique_ptr<PrintStatementAST> ParsePrintStatement(Tokenizer &tokenizer,
                                                       SymbolTable &st);

template <bool isLoop>
std::unique_ptr<IfStatementAST> ParseIfStatement(Tokenizer &tokenizer,
                                                 SymbolTable &st) {
    auto cexpr = ParseExpression(tokenizer, st);

    auto tbranch = ParseConditionalBody<isLoop>(tokenizer, st);

    CheckTokenizer(tokenizer);
    auto [token0, word0] = tokenizer.GetNextToken();
    if (token0 != Else) {
        throw std::runtime_error(
            std::format("Expected keyword else but found {} at pos {}.\n",
                        word0, tokenizer.GetPos()));
    }

    CheckTokenizer(tokenizer);
    auto [token1, word1] = tokenizer.GetNextToken();
    if (token1 != LParen) {
        throw std::runtime_error(
            std::format("Expected {} but found {} at pos {}.\n", "{", word1,
                        tokenizer.GetPos()));
    }

    auto fbranch = ParseConditionalBody<isLoop>(tokenizer, st);

    return make_unique<IfStatementAST>(std::move(cexpr), std::move(tbranch),
                                       std::move(fbranch));
}

template <Token tk>
std::unique_ptr<LoopCtrlStatementAST<tk>> ParseLoopCtrlStatement(
    Tokenizer &tokenizer, SymbolTable &st [[__maybe_unused__]]) {
    CheckTokenizer(tokenizer);
    auto [token, word] = tokenizer.GetNextToken();
    if (token != SColon) {
        throw std::runtime_error(std::format(
            "Expected ; but found {} at pos {}.\n", word, tokenizer.GetPos()));
    }
    return std::make_unique<LoopCtrlStatementAST<tk>>();
}

std::unique_ptr<WhileStatementAST> ParseWhileStatement(Tokenizer &tokenzier,
                                                       SymbolTable &st);

std::unique_ptr<DummyAST> ParseFunction(Tokenizer &tokenizer, SymbolTable &st);

std::unique_ptr<ReturnStatementAST> ParseReturnStatement(Tokenizer &tokenizer,
                                                         SymbolTable &st);

std::unique_ptr<FunctionCallAST> ParseFunctionCall(Tokenizer &tokenizer,
                                                   SymbolTable &st);

template <bool isLoop>
std::vector<std::unique_ptr<BaseAST>> ParseConditionalBody(Tokenizer &tokenizer,
                                                           SymbolTable &st) {
    std::vector<std::unique_ptr<BaseAST>> nodes;

    bool cont = true;
    while (cont) {
        CheckTokenizer(tokenizer);
        auto [token, word] = tokenizer.GetNextToken();
        switch (token) {
            case Let:
                nodes.push_back(ParseDeclaration(tokenizer, st));
                break;
            case Mut:
                nodes.push_back(ParseStatement(tokenizer, st));
                break;
            case Print:
                nodes.push_back(ParsePrintStatement(tokenizer, st));
                break;
            case Break: {
                if constexpr (isLoop) {
                    nodes.push_back(
                        ParseLoopCtrlStatement<Break>(tokenizer, st));
                } else {
                    throw std::runtime_error(
                        std::format("Invalid token: found {} at pos {}.\n",
                                    word, tokenizer.GetPos()));
                }
                break;
            }
            case Continue: {
                if constexpr (isLoop) {
                    nodes.push_back(
                        ParseLoopCtrlStatement<Continue>(tokenizer, st));
                } else {
                    throw std::runtime_error(
                        std::format("Invalid token: found {} at pos {}.\n",
                                    word, tokenizer.GetPos()));
                }
                break;
            }
            case If:
                nodes.push_back(ParseIfStatement<isLoop>(tokenizer, st));
                break;
            case While:
                nodes.push_back(ParseWhileStatement(tokenizer, st));
                break;
            case RParen:
                cont = false;
                break;
            default:
                throw std::runtime_error(
                    std::format("Invalid token: found {} at pos {}.\n", word,
                                tokenizer.GetPos()));
        }
    }

    return nodes;
}

std::vector<std::unique_ptr<BaseAST>> ParseFunctionBody(Tokenizer &tokenizer,
                                                        SymbolTable &st);

std::vector<std::string> ParseArgumentList(Tokenizer &tokenizer,
                                           SymbolTable &st);

}  // namespace sci
