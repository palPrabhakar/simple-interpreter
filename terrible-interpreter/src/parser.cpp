#include "parser.h"

#include <cassert>
#include <memory>
#include <unordered_map>

#include "expr.h"
#include "tokenizer.h"
#include "tokens.h"

namespace tci {

static const std::unordered_map<Token, int> op_precedence = {
    {Add, 20}, {Sub, 20}, {Mul, 40}, {Div, 40}};

void CheckTokenizer(Tokenizer &tokenizer) {
  if (tokenizer.EOP()) {
    assert(false && "Unexpected end of Parsing\n");
  }
}

std::unique_ptr<Expr<int>> ParseExpression(Tokenizer &tokenizer, std::unique_ptr<OpExpr<int, int>> expr) {
  // std::cout << "ParseExpression\n";
  CheckTokenizer(tokenizer);
  auto [vtok, val] = tokenizer.GetNextToken();
  assert(vtok == Text && "Text token expected\n");

  CheckTokenizer(tokenizer);
  auto [ntoken, nword] = tokenizer.GetNextToken();

  if (ntoken == SColon) {
    if(expr) {
      expr->SetRhs(std::make_unique<ValueExpr<int>>(std::stoi(val)));
      return std::move(expr);
    } else {
      return std::make_unique<ValueExpr<int>>(std::stoi(val));
    }
  } else {
    // std::cout<<"op: "<<nword<<std::endl;
    auto nexp =  MakeOpExpr::GetOpExpr<int>(ntoken);

    if(expr) {
      if (expr->GetPrecedence() >= nexp->GetPrecedence()) {
        expr->SetRhs(std::make_unique<ValueExpr<int>>(std::stoi(val)));

        nexp->SetLhs(std::move(expr));

        return ParseExpression(tokenizer, std::move(nexp));

      } else {
        nexp->SetLhs(std::make_unique<ValueExpr<int>>(std::stoi(val)));

        expr->SetRhs(ParseExpression(tokenizer, std::move(nexp)));

        return std::move(expr);
      }
    } else {
        nexp->SetLhs(std::make_unique<ValueExpr<int>>(std::stoi(val)));
        return ParseExpression(tokenizer, std::move(nexp));
    }
  }
}

void ParseStatement(Tokenizer &tokenizer) {
  if (!tokenizer.EOP()) {
    auto [token, word] = tokenizer.GetNextToken();
    if (token == Int) {
      auto [token, word] = tokenizer.GetNextToken();
      assert(token == Text && "Text token expected\n");
      auto varName = word;

      auto [aToken, aWord] = tokenizer.GetNextToken();
      assert(aToken == Assign && "Assign token expected\n");

      auto exp = ParseExpression(tokenizer, nullptr);

      std::cout<<"Result: "<<exp->Operate()<<std::endl;
    }
  }
}

void Parse(Tokenizer &tokenizer) {
  // while (!tokenizer.EOP()) {
  ParseStatement(tokenizer);
  // }
}
}  // namespace tci
