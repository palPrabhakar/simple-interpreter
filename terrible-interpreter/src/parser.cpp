#include "parser.h"

#include <cassert>
#include <cctype>
#include <memory>
#include <unordered_map>

#include "expr.h"
#include "symbol_table.h"
#include "tokenizer.h"
#include "tokens.h"

namespace tci {

void CheckTokenizer(Tokenizer &tokenizer) {
  if (tokenizer.EOP()) {
    assert(false && "Unexpected end of Parsing\n");
  }
}

std::unique_ptr<Expr<int>> ParseExpression(Tokenizer &tokenizer) {
  CheckTokenizer(tokenizer);
  auto [tok0, word0] = tokenizer.GetNextToken();
  std::unique_ptr<Expr<int>> op0;

  if (tok0 == LBrack) {
    op0 = ParseExpression(tokenizer);
  } else if (tok0 == Text) {
    if (std::isalpha(word0[0])) {
      op0 = std::make_unique<VarExpr<int>>(word0);
    } else {
      op0 = std::make_unique<ValueExpr<int>>(std::stoi(word0));
    }
  } else {
    assert(false && "Failed to parse expression\n");
  }

  CheckTokenizer(tokenizer);
  auto [tok1, word1] = tokenizer.GetNextToken();

  if (tok1 == SColon || tok1 == RBrack) {
    return op0;
  } else {
    auto nexp = MakeOpExpr::GetOpExpr<int>(tok1);
    nexp->SetLhs(std::move(op0));
    return ParseExpression(tokenizer, std::move(nexp));
  }
}

std::unique_ptr<Expr<int>> ParseExpression(
    Tokenizer &tokenizer, std::unique_ptr<OpExpr<int, int>> expr) {
  std::unique_ptr<Expr<int>> op0;

  CheckTokenizer(tokenizer);
  auto [tok0, word0] = tokenizer.GetNextToken();

  if (tok0 == LBrack) {
    op0 = ParseExpression(tokenizer);
  } else if (tok0 == Text) {
    if (std::isalpha(word0[0])) {
      op0 = std::make_unique<VarExpr<int>>(word0);
    } else {
      op0 = std::make_unique<ValueExpr<int>>(std::stoi(word0));
    }
  } else {
    assert(false && "Failed to parse expression\n");
  }

  CheckTokenizer(tokenizer);
  auto [tok1, word1] = tokenizer.GetNextToken();

  if (tok1 == SColon || tok1 == RBrack) {
    expr->SetRhs(std::move(op0));
    return std::move(expr);
  } else {
    // std::cout<<"op: "<<nword<<std::endl;
    auto nexp = MakeOpExpr::GetOpExpr<int>(tok1);
    if (expr->GetPrecedence() >= nexp->GetPrecedence()) {
      expr->SetRhs(std::move(op0));
      nexp->SetLhs(std::move(expr));
      return ParseExpression(tokenizer, std::move(nexp));
    } else {
      nexp->SetLhs(std::move(op0));
      expr->SetRhs(ParseExpression(tokenizer, std::move(nexp)));
      return std::move(expr);
    }
  }
}

void ParseStatement(Tokenizer &tokenizer) {
  // CheckTokenizer(tokenizer);
  // auto [token, word] = tokenizer.GetNextToken();

  // if (token == Int) {

  CheckTokenizer(tokenizer);
  auto [token, word] = tokenizer.GetNextToken();
  assert(token == Text && "Text token expected\n");
  auto varName = word;

  CheckTokenizer(tokenizer);
  auto [aToken, aWord] = tokenizer.GetNextToken();
  assert(aToken == Assign && "Assign token expected\n");

  auto exp = ParseExpression(tokenizer);

  auto &st = SymbolTable::GetInstance();

  if (st.symbols.contains(word)) {
    assert(false && "Redeclaration of variable\n");
  }

  st.symbols.insert({word, Symbol{Int, exp->Operate()}});
  std::cout << "Result: " << std::get<int>(st.symbols[word].val) << std::endl;
  // }
  // }
}

std::vector<std::pair<Token, std::string>> ParseArgumentList(
    Tokenizer &tokenizer) {
  CheckTokenizer(tokenizer);
  auto [token0, word0] = tokenizer.GetNextToken();
  assert(token0 == LBrack && "Expected (\n.");

  std::vector<std::pair<Token, std::string>> args;

  while (true) {
    CheckTokenizer(tokenizer);
    auto [token0, word0] = tokenizer.GetNextToken();

    if(token0 == RBrack) {
      break;
    }

    assert(token0 == Int && "Expected Type keyword\n.");

    CheckTokenizer(tokenizer);
    auto [token1, word1] = tokenizer.GetNextToken();
    assert(token1 == Text && "Expected Variable Name\n.");

    args.emplace_back(token0, word1);
  }

  return args;
}

void ParseFunction(Tokenizer &tokenizer) {
  CheckTokenizer(tokenizer);
  auto [token0, word0] = tokenizer.GetNextToken();
  assert(token0 == Int && "Only Int type supported for now\n");

  CheckTokenizer(tokenizer);
  auto [token1, word1] = tokenizer.GetNextToken();
  assert(token1 == Text && "Function name expected\n");

  auto args = ParseArgumentList(tokenizer);

  for(auto &[token, word] : args) {
    std::cout<<word<<std::endl;
  }

  // ParseFunctionBody(tokenizer);
}

void Parse(Tokenizer &tokenizer) {
  CheckTokenizer(tokenizer);
  auto [token, word] = tokenizer.GetNextToken();
  switch (token) {
    case Int:
      ParseStatement(tokenizer);
      break;
    case Fn:
      ParseFunction(tokenizer);
      break;
    default:
      assert(false && "Invalid token\n");
  }
}
}  // namespace tci
