#include "parser.h"

#include <cctype>
#include <memory>
#include <stdexcept>

#include "ast.h"
#include "expr.h"
#include "function.h"
#include "optimizers/register_alloc.hpp"
#include "statement.h"
#include "symbol_table.h"
#include "tokenizer.h"
#include "tokens.h"

namespace sci {

std::unique_ptr<ExprAST> ParseExpression(Tokenizer &tokenizer,
                                         SymbolTable &st) {
    CheckTokenizer(tokenizer);
    auto [tok0, word0] = tokenizer.GetNextToken();
    std::unique_ptr<ExprAST> op0;

    switch (tok0) {
        case LBrack:
            op0 = ParseExpression(tokenizer, st);
            break;
        case Text: {
            if (std::isalpha(word0[0])) {
                if (!st.CheckSymbol(word0)) {
                    throw std::runtime_error(std::format(
                        "ParseExpression: Use of undefined variable {} at {}\n",
                        word0, tokenizer.GetPos()));
                }
                op0 = std::make_unique<VarAST>(word0);
            } else {
                op0 = std::make_unique<ValueAST>(word0);
            }
            break;
        }
        case Call: {
            op0 = ParseFunctionCall(tokenizer, st);
            break;
        }
        default:
            throw std::runtime_error(
                "ParseExpression: Failed to parse expression.\n");
    }

    CheckTokenizer(tokenizer);
    auto [tok1, word1] = tokenizer.GetNextToken();

    if (tok1 == SColon || tok1 == RBrack || tok1 == LParen) {
        return op0;
    } else {
        auto nexp = std::make_unique<OpAST>(tok1, word1);
        nexp->SetLhs(std::move(op0));
        return ParseExpression(tokenizer, std::move(nexp), st);
    }
}

std::unique_ptr<ExprAST> ParseExpression(Tokenizer &tokenizer,
                                         std::unique_ptr<OpAST> expr,
                                         SymbolTable &st) {
    std::unique_ptr<ExprAST> op0;

    CheckTokenizer(tokenizer);
    auto [tok0, word0] = tokenizer.GetNextToken();

    switch (tok0) {
        case LBrack: {
            op0 = ParseExpression(tokenizer, st);
            break;
        }
        case Text: {
            if (std::isalpha(word0[0])) {
                if (!st.CheckSymbol(word0)) {
                    throw std::runtime_error(std::format(
                        "ParseExpression: Use of undefined variable {} at {}\n",
                        word0, tokenizer.GetPos()));
                }
                op0 = std::make_unique<VarAST>(word0);
            } else {
                op0 = std::make_unique<ValueAST>(word0);
            }
            break;
        }
        case Call: {
            if (std::isalpha(word0[0])) {
                op0 = ParseFunctionCall(tokenizer, st);
                break;
            }
        }
        default:
            throw std::runtime_error(
                "ParseExpression: Failed to parse expression.\n");
    }

    CheckTokenizer(tokenizer);
    auto [tok1, word1] = tokenizer.GetNextToken();

    if (tok1 == SColon || tok1 == RBrack) {
        expr->SetRhs(std::move(op0));
        return expr;
    } else {
        auto nexp = std::make_unique<OpAST>(tok1, word1);
        if (expr->GetPrecedence() >= nexp->GetPrecedence()) {
            expr->SetRhs(std::move(op0));
            nexp->SetLhs(std::move(expr));
            return ParseExpression(tokenizer, std::move(nexp), st);
        } else {
            nexp->SetLhs(std::move(op0));
            expr->SetRhs(ParseExpression(tokenizer, std::move(nexp), st));
            return expr;
        }
    }
}

std::unique_ptr<FunctionCallAST> ParseFunctionCall(Tokenizer &tokenizer,
                                                   SymbolTable &st) {
    CheckTokenizer(tokenizer);
    auto [tk_fname, fname] = tokenizer.GetNextToken();
    if (tk_fname != Text) {
        throw std::runtime_error(
            std::format("ParseFunctionCall: Expected function name but found "
                        "{} at pos {}.\n",
                        fname, tokenizer.GetPos()));
    }

    if (!st.CheckFunction(fname)) {
        throw std::runtime_error(std::format(
            "ParseFunctionCall: Calling undefined function  {}.\n", fname));
    }

    CheckTokenizer(tokenizer);
    auto [token1, word1] = tokenizer.GetNextToken();
    if (token1 != LBrack) {
        throw std::runtime_error(std::format(
            "ParseFunctionCall: Expected ( but found {} at pos {}.\n", word1,
            tokenizer.GetPos()));
    }

    // TODO:
    // Do check for function signature here
    // Semantic thing? makes more sense here then
    // code gen??
    std::vector<std::unique_ptr<ExprAST>> args;
    bool comma = false;
    while (true) {
        CheckTokenizer(tokenizer);
        auto [token, word] = tokenizer.GetNextToken();

        if (token == RBrack) {
            break;
        }

        if (token == Comma && comma) {
            comma = false;
            continue;
        } else if (token == Comma && !comma) {
            throw std::runtime_error(
                std::format("Expected function parameter but found ',' at {}\n",
                            tokenizer.GetPos()));
        }

        if (std::isalpha(word[0])) {
            if (!st.CheckSymbol(word)) {
                throw std::runtime_error(std::format(
                    "ParseFunctionCall: Use of undefined variable {} at {}\n",
                    word, tokenizer.GetPos()));
            }
            args.push_back(std::make_unique<VarAST>(word));
        } else {
            args.push_back(std::make_unique<ValueAST>(word));
        }
        comma = true;
    }

    auto &fn_proto = st.GetPrototype(fname);
    if (args.size() != fn_proto->GetArgumentSize()) {
        throw std::runtime_error(std::format(
            "ParseFunctionCall: Argument size mismatch. Expected {} "
            "arguments but found {}.\n",
            fn_proto->GetArgumentSize(), args.size()));
    }

    // Get to this point only if valid function call
    return std::make_unique<FunctionCallAST>(fname, std::move(args));
}

std::unique_ptr<StatementAST> ParseStatement(Tokenizer &tokenizer,
                                             SymbolTable &st) {
    CheckTokenizer(tokenizer);
    auto [token, word] = tokenizer.GetNextToken();
    if (token != Text) {
        throw std::runtime_error(std::format(
            "ParseStatement: Expected variable name but found {} at pos {}.\n",
            word, tokenizer.GetPos()));
    }
    auto varName = word;

    CheckTokenizer(tokenizer);
    auto [aToken, aWord] = tokenizer.GetNextToken();
    if (aToken != Assign) {
        throw std::runtime_error(
            std::format("ParseStatement: Expected = but found {} at pos {}.\n",
                        aWord, tokenizer.GetPos()));
    }

    auto expr = ParseExpression(tokenizer, st);

    if (!st.CheckSymbol(word)) {
        throw std::runtime_error(std::format(
            "ParseStatement: Use of undefined variable {}.\n", word));
    }

    return std::make_unique<StatementAST>(word, std::move(expr));
}

std::unique_ptr<StatementAST> ParseDeclaration(Tokenizer &tokenizer,
                                               SymbolTable &st) {
    CheckTokenizer(tokenizer);
    auto [token, word] = tokenizer.GetNextToken();
    if (token != Text) {
        throw std::runtime_error(
            std::format("ParseDeclaration: Expected variable name but found {} "
                        "at pos {}.\n",
                        word, tokenizer.GetPos()));
    }
    auto varName = word;

    CheckTokenizer(tokenizer);
    auto [aToken, aWord] = tokenizer.GetNextToken();
    if (aToken != Assign) {
        throw std::runtime_error(std::format(
            "ParseDeclaration: Expected = but found {} at pos {}.\n", aWord,
            tokenizer.GetPos()));
    }

    auto expr = ParseExpression(tokenizer, st);

    if (st.CheckSymbol(word)) {
        throw std::runtime_error(std::format(
            "ParseDeclaration: Redeclaration of variable {}.\n", word));
    }

    st.InsertSymbol(word);

    return std::make_unique<StatementAST>(word, std::move(expr));
}

std::unique_ptr<DummyAST> ParseFunction(Tokenizer &tokenizer, SymbolTable &st) {
    CheckTokenizer(tokenizer);
    auto [token, fn_name] = tokenizer.GetNextToken();
    if (token != Text) {
        throw std::runtime_error(std::format(
            "ParseFunction: Expected function name but found {} at pos {}.\n",
            fn_name, tokenizer.GetPos()));
    }

    if (st.CheckFunction(fn_name)) {
        throw std::runtime_error(std::format(
            "ParseFunction: Redefinition of function {}.\n", fn_name));
    }

    st.PushSymbolTable();

    auto args = ParseArgumentList(tokenizer, st);
    auto body = ParseFunctionBody(tokenizer, st);
    auto ret = ParseReturnStatement(tokenizer, st);

    CheckTokenizer(tokenizer);
    auto [token1, word1] = tokenizer.GetNextToken();
    if (token1 != RParen) {
        throw std::runtime_error(
            std::format("ParseFunction: Expected {} but found {} at pos {}.\n",
                        "}", word1, tokenizer.GetPos()));
    }

    auto arg_size = args.size();
    auto fptr = std::make_unique<FunctionAST>(
        fn_name, std::move(args), std::move(body), std::move(ret));
    auto code = fptr->GenerateCode(st);
    code = do_register_alloc(std::move(code));

    st.PopSymbolTable();

    st.InsertPrototype(fn_name, std::make_unique<FunctionPrototype>(
                                    fn_name, arg_size, std::move(code)));

    return std::make_unique<DummyAST>(fn_name);
}

std::unique_ptr<ReturnStatementAST> ParseReturnStatement(Tokenizer &tokenizer,
                                                         SymbolTable &st) {
    auto expr = ParseExpression(tokenizer, st);

    return std::make_unique<ReturnStatementAST>(std::move(expr));
}

std::vector<std::string> ParseArgumentList(Tokenizer &tokenizer,
                                           SymbolTable &st) {
    CheckTokenizer(tokenizer);
    auto [token0, word0] = tokenizer.GetNextToken();
    if (token0 != LBrack) {
        throw std::runtime_error(std::format(
            "ParseArgumentList: Expected ( but found {} at pos {}.\n", word0,
            tokenizer.GetPos()));
    }

    std::vector<std::string> args;

    CheckTokenizer(tokenizer);
    auto [token, word] = tokenizer.GetNextToken();

    if (token != RBrack) {  // empty function with no parameters
        while (true) {
            if (token != Text) {
                throw std::runtime_error(std::format(
                    "ParseArgumentList: Expected variable name but found "
                    "{} at pos {}.\n",
                    word, tokenizer.GetPos()));
            }

            // don't want to check recursively
            if (st.CheckSymbol<true>(word)) {
                throw std::runtime_error(std::format(
                    "ParseArgumentList: Redeclaration of variable {} at {}\n",
                    word, tokenizer.GetPos()));
            }

            st.InsertSymbol(word);
            args.push_back(word);

            CheckTokenizer(tokenizer);
            auto [token0, word0] = tokenizer.GetNextToken();

            if (token0 == RBrack) {
                break;
            }

            if (token0 != Comma) {
                throw std::runtime_error(
                    std::format("ParseArgumentList: Expected , or ) but found "
                                "{} at pos {}.\n",
                                word0, tokenizer.GetPos()));
            }

            auto tok = tokenizer.GetNextToken();
            token = tok.first;
            word = tok.second;
        }
    }

    return args;
}

std::unique_ptr<PrintStatementAST> ParsePrintStatement(Tokenizer &tokenizer,
                                                       SymbolTable &st) {
    CheckTokenizer(tokenizer);
    auto [token, word] = tokenizer.GetNextToken();
    // First token should be '('
    if (token != LBrack) {
        throw std::runtime_error(std::format(
            "ParsePrintStatment: Expected ( but found {} at pos {}.\n", word,
            tokenizer.GetPos()));
    }

    std::vector<std::unique_ptr<ExprAST>> op;
    bool comma = false;
    while (true) {
        CheckTokenizer(tokenizer);
        auto [token, word] = tokenizer.GetNextToken();

        if (token == RBrack) {
            break;
        }

        if (token == Comma && comma) {
            comma = false;
            continue;
        } else if (token == Comma && !comma) {
            throw std::runtime_error(
                std::format("Expected parameter but found ',' at {}\n",
                            tokenizer.GetPos()));
        }

        if (token == LBrack) {
            op.push_back(ParseExpression(tokenizer, st));
        } else if (std::isalpha(word[0])) {
            if (!st.CheckSymbol(word)) {
                throw std::runtime_error(std::format(
                    "ParsePrintStatement: Use of undefined variable {} at {}\n",
                    word, tokenizer.GetPos()));
            }
            op.push_back(std::make_unique<VarAST>(word));
        } else {
            op.push_back(std::make_unique<ValueAST>(word));
        }
        comma = true;
    }

    CheckTokenizer(tokenizer);
    auto [etoken, eword] = tokenizer.GetNextToken();
    if (etoken != SColon) {
        throw std::runtime_error(std::format(
            "Error ParsingPrintStatement: Expected ';' but found {} at {}\n",
            eword, tokenizer.GetPos()));
    }

    return std::make_unique<PrintStatementAST>(std::move(op));
}

// single return statement
std::vector<std::unique_ptr<BaseAST>> ParseFunctionBody(Tokenizer &tokenizer,
                                                        SymbolTable &st) {
    CheckTokenizer(tokenizer);
    auto [token0, word0] = tokenizer.GetNextToken();
    if (token0 != LParen) {
        throw std::runtime_error(std::format(
            "ParseFunctionBody: Expected {} but found {} at pos {}.\n", "{",
            word0, tokenizer.GetPos()));
    }

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
            case Print:
                nodes.push_back(ParsePrintStatement(tokenizer, st));
                break;
            case If:
                nodes.push_back(ParseIfStatement<false>(tokenizer, st));
                break;
            case While:
                nodes.push_back(ParseWhileStatement(tokenizer, st));
                break;
            case Return:
                cont = false;
                break;
            default:
                throw std::runtime_error(std::format(
                    "ParseFunctionBody: Invalid token: found {} at pos {}.\n",
                    word, tokenizer.GetPos()));
        }
    }

    return nodes;
}

std::unique_ptr<WhileStatementAST> ParseWhileStatement(Tokenizer &tokenizer,
                                                       SymbolTable &st) {
    auto cexpr = ParseExpression(tokenizer, st);

    bool cont = true;
    std::vector<std::unique_ptr<BaseAST>> nodes;
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
            case Break:
                nodes.push_back(ParseLoopCtrlStatement<Break>(tokenizer, st));
                break;
            case Continue:
                nodes.push_back(
                    ParseLoopCtrlStatement<Continue>(tokenizer, st));
                break;
            case If:
                nodes.push_back(ParseIfStatement<true>(tokenizer, st));
                break;
            case While:
                nodes.push_back(ParseWhileStatement(tokenizer, st));
                break;
            case RParen:
                cont = false;
                break;
            default:
                throw std::runtime_error(std::format(
                    "ParseWhileStatement: Invalid token: found {} at pos {}.\n",
                    word, tokenizer.GetPos()));
        }
    }

    return std::make_unique<WhileStatementAST>(std::move(cexpr),
                                               std::move(nodes));
}

std::unique_ptr<BaseAST> Parse(Tokenizer &tokenizer, SymbolTable &st) {
    if (!tokenizer.EOP()) {
        auto [token, word] = tokenizer.GetNextToken();
        std::unique_ptr<BaseAST> ast;
        switch (token) {
            case Let:
                ast = ParseDeclaration(tokenizer, st);
                break;
            case Mut:
                ast = ParseStatement(tokenizer, st);
                break;
            case Fn:
                ast = ParseFunction(tokenizer, st);
                break;
            case While:
                ast = ParseWhileStatement(tokenizer, st);
                break;
            case If:
                ast = ParseIfStatement<false>(tokenizer, st);
                break;
            case Print:
                ast = ParsePrintStatement(tokenizer, st);
                break;
            default:
                throw std::runtime_error(
                    std::format("Parse: Invalid token: found {} at pos {}.\n",
                                word, tokenizer.GetPos()));
        }
        return ast;
    }
    return nullptr;
}
}  // namespace sci
