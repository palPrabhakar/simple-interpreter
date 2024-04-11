#include "parser.h"

#include <cassert>
#include <cctype>
#include <memory>
#include <unordered_map>

#include "expr.h"
#include "statement.h"
#include "symbol_table.h"
#include "tokenizer.h"
#include "tokens.h"

namespace tci {

inline void CheckTokenizer(Tokenizer &tokenizer) {
  if (tokenizer.EOP()) {
    assert(false && "Unexpected end of Parsing\n");
  }
}

std::unique_ptr<ExprAST> ParseExpression(Tokenizer &tokenizer,
                                         SymbolTable &st) {
  CheckTokenizer(tokenizer);
  auto [tok0, word0] = tokenizer.GetNextToken();
  std::unique_ptr<ExprAST> op0;

  if (tok0 == LBrack) {
    op0 = ParseExpression(tokenizer, st);
  } else if (tok0 == Text) {
    if (std::isalpha(word0[0])) {
      assert(st.CheckSymbol(word0) && "Undefined variable");
      op0 = std::make_unique<VarAST>(word0);
    } else {
      op0 = std::make_unique<ValueAST>(word0);
    }
  } else {
    assert(false && "Failed to parse expression\n");
  }

  CheckTokenizer(tokenizer);
  auto [tok1, word1] = tokenizer.GetNextToken();

  if (tok1 == SColon || tok1 == RBrack) {
    return op0;
  } else {
    auto nexp = std::make_unique<OpAST>(tok1, word1);
    nexp->SetLhs(std::move(op0));
    return ParseExpression(tokenizer, std::move(nexp), st);
  }
}

std::unique_ptr<ExprAST> ParseExpression(Tokenizer &tokenizer,
                                         std::unique_ptr<ExprAST> expr,
                                         SymbolTable &st) {
  std::unique_ptr<ExprAST> op0;

  CheckTokenizer(tokenizer);
  auto [tok0, word0] = tokenizer.GetNextToken();

  if (tok0 == LBrack) {
    op0 = ParseExpression(tokenizer, st);
  } else if (tok0 == Text) {
    if (std::isalpha(word0[0])) {
      assert(st.CheckSymbol(word0) && "Undefined variable");
      op0 = std::make_unique<VarAST>(word0);
    } else {
      op0 = std::make_unique<ValueAST>(word0);
    }
  } else {
    assert(false && "Failed to parse expression\n");
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

std::unique_ptr<StatementAST> ParseStatement(Tokenizer &tokenizer,
                                             SymbolTable &st) {
  CheckTokenizer(tokenizer);
  auto [token, word] = tokenizer.GetNextToken();
  assert(token == Text && "Text token expected\n");
  auto varName = word;

  CheckTokenizer(tokenizer);
  auto [aToken, aWord] = tokenizer.GetNextToken();
  assert(aToken == Assign && "Assign token expected\n");

  auto expr = ParseExpression(tokenizer, st);

  if (!st.CheckSymbol(word)) {
    assert(false && "Undefined variable\n");
  }

  return std::make_unique<StatementAST>(word, std::move(expr));
}

std::unique_ptr<StatementAST> ParseDeclaration(Tokenizer &tokenizer,
                                               SymbolTable &st) {
  std::cout << "ParseDecl\n";
  CheckTokenizer(tokenizer);
  auto [token, word] = tokenizer.GetNextToken();
  assert(token == Text && "Text token expected\n");
  auto varName = word;

  CheckTokenizer(tokenizer);
  auto [aToken, aWord] = tokenizer.GetNextToken();
  assert(aToken == Assign && "Assign token expected\n");

  auto expr = ParseExpression(tokenizer, st);

  if (st.CheckSymbol(word)) {
    assert(false && "Redeclaration of variable\n");
  }

  st.symbols.insert({word, 0});

  return std::make_unique<StatementAST>(word, std::move(expr));
}

void ParseIfStatement(Tokenizer &tokenizer, SymbolTable &st) {
  std::cout << "Parse If Statement\n" << std::endl;
}

std::vector<std::string> ParseArgumentList(Tokenizer &tokenizer,
                                           SymbolTable &st) {
  CheckTokenizer(tokenizer);
  auto [token, word] = tokenizer.GetNextToken();
  assert(token == LBrack && "Expected (\n.");

  std::vector<std::string> args;

  while (true) {
    CheckTokenizer(tokenizer);
    auto [token, word] = tokenizer.GetNextToken();

    if (token == RBrack) {
      break;
    }

    if (token == Comma) {
      continue;
    }

    assert(token == Text && "Expected Variable Name\n.");

    // don't want to check recursively
    if (st.symbols.contains(word)) {
      assert(false && "Redecleration of function parameter");
    }

    st.symbols.insert({word, 0});
    args.push_back(word);
  }

  return args;
}

void ParseFunction(Tokenizer &tokenizer, SymbolTable &st) {
  CheckTokenizer(tokenizer);
  auto [token, word] = tokenizer.GetNextToken();
  assert(token == Text && "Function name expected\n");

  std::string fn_name = word;

  // functions symbol table
  SymbolTable fst(&st);

  auto args = ParseArgumentList(tokenizer, fst);

  for (auto arg : args) {
    std::cout << arg << std::endl;
  }

  auto body = ParseFunctionBody(tokenizer, fst);
}

// single return statement
std::vector<std::unique_ptr<BaseAST>> ParseFunctionBody(Tokenizer &tokenizer,
                                                        SymbolTable &st) {
  CheckTokenizer(tokenizer);
  auto [token0, word0] = tokenizer.GetNextToken();
  assert(token0 == LParen && "Expected token {\n");

  std::vector<std::unique_ptr<BaseAST>> nodes;

  bool cont = true;
  while (cont) {
    CheckTokenizer(tokenizer);
    auto [token, word] = tokenizer.GetNextToken();
    switch (token) {
      case Return: {
        nodes.push_back(ParseExpression(tokenizer, st));
        cont = false;
        break;
      }
      default:
        assert(false && "Invalid token\n");
    }
  }

  CheckTokenizer(tokenizer);
  auto [token1, word1] = tokenizer.GetNextToken();
  assert(token1 == RParen && "Expected token }\n");

  return nodes;
}

void Parse(Tokenizer &tokenizer, SymbolTable &st) {
  CheckTokenizer(tokenizer);
  auto [token, word] = tokenizer.GetNextToken();
  switch (token) {
    case Let: {
      auto ast = ParseDeclaration(tokenizer, st);
      uint ridx = 0;
      auto operations = ast->GenerateCode(ridx);
      for (auto op : operations) {
        std::cout << op << std::endl;
      }
    } break;
    case Mut: {
      auto ast = ParseStatement(tokenizer, st);
      uint ridx = 0;
      auto operations = ast->GenerateCode(ridx);
      for (auto op : operations) {
        std::cout << op << std::endl;
      }
    } break;
    case Fn:
      ParseFunction(tokenizer, st);
      break;
    case If:
      ParseIfStatement(tokenizer, st);
      break;
    default:
      assert(false && "Invalid token\n");
  }
}
}  // namespace tci
