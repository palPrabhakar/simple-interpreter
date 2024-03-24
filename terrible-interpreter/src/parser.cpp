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

// static const std::unordered_map<Token, int> op_precedence = {
//     {Add, 20}, {Sub, 20}, {Mul, 40}, {Div, 40}};

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
    if(std::isalpha(word0[0])) {
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
    if(std::isalpha(word0[0])) {
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
    CheckTokenizer(tokenizer);
    auto [token, word] = tokenizer.GetNextToken();

    if (token == Int) {

      CheckTokenizer(tokenizer);
      auto [token, word] = tokenizer.GetNextToken();
      assert(token == Text && "Text token expected\n");
      auto varName = word;

      CheckTokenizer(tokenizer);
      auto [aToken, aWord] = tokenizer.GetNextToken();
      assert(aToken == Assign && "Assign token expected\n");

      auto exp = ParseExpression(tokenizer);

      auto &st = SymbolTable::GetInstance();
      st.symbols.insert({word, Symbol{Int, exp->Operate()}});

      std::cout << "Result: " << std::get<int>(st.symbols[word].val) << std::endl;
    }
  // }
}

void Parse(Tokenizer &tokenizer) {
  // while (!tokenizer.EOP()) {
  ParseStatement(tokenizer);
  // }
}
}  // namespace tci
