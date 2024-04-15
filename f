[1mdiff --git a/README.md b/README.md[m
[1mindex ea05944..9b75bcf 100644[m
[1m--- a/README.md[m
[1m+++ b/README.md[m
[36m@@ -1,6 +1,6 @@[m
 # Terrible Interpreter[m
 [m
 1. No support for for-loops.[m
[31m-2. Only one return statement in the function body[m
[31m-3. Mandatory else clause in if-else-statement[m
[31m-4. Support for only two scope level - Global and Function[m
[32m+[m[32m2. Only one return statement in the function body.[m
[32m+[m[32m3. Mandatory else clause in if-else-statement.[m
[32m+[m[32m4. Support for only two scope level - Global and Function.[m
[1mdiff --git a/includes/function.h b/includes/function.h[m
[1mindex f7f2e3d..ddefeee 100644[m
[1m--- a/includes/function.h[m
[1m+++ b/includes/function.h[m
[36m@@ -18,16 +18,16 @@[m [mclass ReturnStatementAST : public BaseAST {[m
   std::unique_ptr<ExprAST> m_expr;[m
 };[m
 [m
[31m-class ArgumentAST : public BaseAST {[m
[31m- public:[m
[31m-  ArgumentAST(std::string name) : m_name(name) {}[m
[32m+[m[32m// class ArgumentAST : public BaseAST {[m
[32m+[m[32m//  public:[m
[32m+[m[32m//   ArgumentAST(std::string name) : m_name(name) {}[m
 [m
[31m-  std::vector<std::string> GenerateCodeStr(uint &ridx);[m
[31m-  std::vector<Instruction> GenerateCode(uint &ridx);[m
[32m+[m[32m//   std::vector<std::string> GenerateCodeStr(uint &ridx);[m
[32m+[m[32m//   std::vector<Instruction> GenerateCode(uint &ridx);[m
 [m
[31m- private:[m
[31m-  std::string m_name;[m
[31m-};[m
[32m+[m[32m//  private:[m
[32m+[m[32m//   std::string m_name;[m
[32m+[m[32m// };[m
 [m
 class FunctionAST : public BaseAST {[m
  public:[m
[36m@@ -54,4 +54,21 @@[m [mclass FunctionAST : public BaseAST {[m
   std::vector<std::unique_ptr<BaseAST>> m_body;[m
 };[m
 [m
[32m+[m[32mclass FunctionCallAST : public ExprAST {[m
[32m+[m[32m public:[m
[32m+[m[32m  FunctionCallAST(std::string name, std::vector<std::unique_ptr<ExprAST>> args, std::vector<Instruction> body)[m
[32m+[m[32m      : m_name(name), m_args(std::move(args)), m_body(std::move(body)) {}[m
[32m+[m
[32m+[m[32m  uint GetValue() { return reg; }[m
[32m+[m
[32m+[m[32m  std::vector<std::string> GenerateCodeStr(uint &ridx) { return {}; }[m
[32m+[m[32m  std::vector<Instruction> GenerateCode(uint &ridx) { return {}; }[m
[32m+[m
[32m+[m[32m private:[m
[32m+[m[32m  std::string m_name;[m
[32m+[m[32m  std::vector<std::unique_ptr<ExprAST>> m_args;[m
[32m+[m[32m  std::vector<Instruction> m_body;[m
[32m+[m[32m  uint reg;[m
[32m+[m[32m};[m
[32m+[m
 }  // namespace tci[m
[1mdiff --git a/includes/parser.h b/includes/parser.h[m
[1mindex 9bd154b..9de6aad 100644[m
[1m--- a/includes/parser.h[m
[1m+++ b/includes/parser.h[m
[36m@@ -32,6 +32,8 @@[m [mstd::unique_ptr<FunctionAST> ParseFunction(Tokenizer &tokenizer, SymbolTable &st[m
 [m
 std::unique_ptr<ReturnStatementAST> ParseReturnStatement(Tokenizer &tokenizer, SymbolTable &st);[m
 [m
[32m+[m[32mstd::unique_ptr<FunctionCallAST> ParseFunctionCall(Tokenizer &tokenizer, SymbolTable &st);[m
[32m+[m
 std::vector<std::unique_ptr<BaseAST>> ParseConditionalBody(Tokenizer &tokenizer, SymbolTable &st);[m
 [m
 std::vector<std::unique_ptr<BaseAST>> ParseFunctionBody(Tokenizer &tokenizer, SymbolTable &st);[m
[1mdiff --git a/includes/prototype.h b/includes/prototype.h[m
[1mindex 8c2be69..12cfef9 100644[m
[1m--- a/includes/prototype.h[m
[1m+++ b/includes/prototype.h[m
[36m@@ -11,6 +11,8 @@[m [mclass FunctionPrototype {[m
   FunctionPrototype(std::vector<std::string> args, std::vector<Instruction> fn)[m
       : m_args(std::move(args)), m_fn(std::move(fn)) {}[m
 [m
[32m+[m[32m  std::vector<Instruction> GetInstructions() const { return m_fn; }[m
[32m+[m
  private:[m
   std::vector<std::string> m_args;[m
   std::vector<Instruction> m_fn;[m
[1mdiff --git a/includes/symbol_table.h b/includes/symbol_table.h[m
[1mindex 6cac09a..86d485c 100644[m
[1m--- a/includes/symbol_table.h[m
[1m+++ b/includes/symbol_table.h[m
[36m@@ -34,6 +34,12 @@[m [mclass SymbolTable {[m
   void InsertFunction(std::string name,[m
                       std::unique_ptr<FunctionPrototype> func);[m
 [m
[32m+[m
[32m+[m
[32m+[m[32m  FunctionPrototype *GetPrototype(std::string name) {[m
[32m+[m[32m    return m_functions[name].get();[m
[32m+[m[32m  }[m
[32m+[m
  private:[m
   std::stack<Symbols> m_st;[m
   Symbols g_symbols;  // global symbol space[m
[1mdiff --git a/includes/tokens.h b/includes/tokens.h[m
[1mindex 7dd3dc3..ac97ff4 100644[m
[1m--- a/includes/tokens.h[m
[1m+++ b/includes/tokens.h[m
[36m@@ -42,6 +42,7 @@[m [menum Token {[m
   SColon,[m
   Comma,[m
   Fn,[m
[32m+[m[32m  Call,[m
   End[m
 };[m
 [m
[1mdiff --git a/src/parser.cpp b/src/parser.cpp[m
[1mindex df4df47..2b8e393 100644[m
[1m--- a/src/parser.cpp[m
[1m+++ b/src/parser.cpp[m
[36m@@ -27,19 +27,41 @@[m [mstd::unique_ptr<ExprAST> ParseExpression(Tokenizer &tokenizer,[m
   auto [tok0, word0] = tokenizer.GetNextToken();[m
   std::unique_ptr<ExprAST> op0;[m
 [m
[31m-  if (tok0 == LBrack) {[m
[31m-    op0 = ParseExpression(tokenizer, st);[m
[31m-  } else if (tok0 == Text) {[m
[31m-    if (std::isalpha(word0[0])) {[m
[31m-      assert(st.CheckSymbol(word0) && "Undefined variable");[m
[31m-      op0 = std::make_unique<VarAST>(word0);[m
[31m-    } else {[m
[31m-      op0 = std::make_unique<ValueAST>(word0);[m
[32m+[m[32m  switch (tok0) {[m
[32m+[m[32m    case LBrack:[m
[32m+[m[32m      op0 = ParseExpression(tokenizer, st);[m
[32m+[m[32m      break;[m
[32m+[m[32m    case Text: {[m
[32m+[m[32m      if (std::isalpha(word0[0])) {[m
[32m+[m[32m        assert(st.CheckSymbol(word0) && "Undefined variable");[m
[32m+[m[32m        op0 = std::make_unique<VarAST>(word0);[m
[32m+[m[32m      } else {[m
[32m+[m[32m        op0 = std::make_unique<ValueAST>(word0);[m
[32m+[m[32m      }[m
[32m+[m[32m      break;[m
     }[m
[31m-  } else {[m
[31m-    assert(false && "Failed to parse expression\n");[m
[32m+[m[32m    case Call: {[m
[32m+[m[32m      std::cout << "Call" << std::endl;[m
[32m+[m[32m      op0 = ParseFunctionCall(tokenizer, st);[m
[32m+[m[32m      break;[m
[32m+[m[32m    }[m
[32m+[m[32m    default:[m
[32m+[m[32m      assert(false && "Failed to parse expression\n");[m
   }[m
 [m
[32m+[m[32m  // if (tok0 == LBrack) {[m
[32m+[m[32m  //   op0 = ParseExpression(tokenizer, st);[m
[32m+[m[32m  // } else if (tok0 == Text) {[m
[32m+[m[32m  //   if (std::isalpha(word0[0])) {[m
[32m+[m[32m  //     assert(st.CheckSymbol(word0) && "Undefined variable");[m
[32m+[m[32m  //     op0 = std::make_unique<VarAST>(word0);[m
[32m+[m[32m  //   } else {[m
[32m+[m[32m  //     op0 = std::make_unique<ValueAST>(word0);[m
[32m+[m[32m  //   }[m
[32m+[m[32m  // } else {[m
[32m+[m[32m  //   assert(false && "Failed to parse expression\n");[m
[32m+[m[32m  // }[m
[32m+[m
   CheckTokenizer(tokenizer);[m
   auto [tok1, word1] = tokenizer.GetNextToken();[m
 [m
[36m@@ -60,19 +82,39 @@[m [mstd::unique_ptr<ExprAST> ParseExpression(Tokenizer &tokenizer,[m
   CheckTokenizer(tokenizer);[m
   auto [tok0, word0] = tokenizer.GetNextToken();[m
 [m
[31m-  if (tok0 == LBrack) {[m
[31m-    op0 = ParseExpression(tokenizer, st);[m
[31m-  } else if (tok0 == Text) {[m
[31m-    if (std::isalpha(word0[0])) {[m
[31m-      assert(st.CheckSymbol(word0) && "Undefined variable");[m
[31m-      op0 = std::make_unique<VarAST>(word0);[m
[31m-    } else {[m
[31m-      op0 = std::make_unique<ValueAST>(word0);[m
[32m+[m[32m  switch (tok0) {[m
[32m+[m[32m    case LBrack:[m
[32m+[m[32m      op0 = ParseExpression(tokenizer, st);[m
[32m+[m[32m      break;[m
[32m+[m[32m    case Text: {[m
[32m+[m[32m      if (std::isalpha(word0[0])) {[m
[32m+[m[32m        assert(st.CheckSymbol(word0) && "Undefined variable");[m
[32m+[m[32m        op0 = std::make_unique<VarAST>(word0);[m
[32m+[m[32m      } else {[m
[32m+[m[32m        op0 = std::make_unique<ValueAST>(word0);[m
[32m+[m[32m      }[m
[32m+[m[32m      break;[m
     }[m
[31m-  } else {[m
[31m-    assert(false && "Failed to parse expression\n");[m
[32m+[m[32m    case Call:[m
[32m+[m[32m      op0 = ParseFunctionCall(tokenizer, st);[m
[32m+[m[32m      break;[m
[32m+[m[32m    default:[m
[32m+[m[32m      assert(false && "Failed to parse expression\n");[m
   }[m
 [m
[32m+[m[32m  // if (tok0 == LBrack) {[m
[32m+[m[32m  //   op0 = ParseExpression(tokenizer, st);[m
[32m+[m[32m  // } else if (tok0 == Text) {[m
[32m+[m[32m  //   if (std::isalpha(word0[0])) {[m
[32m+[m[32m  //     assert(st.CheckSymbol(word0) && "Undefined variable");[m
[32m+[m[32m  //     op0 = std::make_unique<VarAST>(word0);[m
[32m+[m[32m  //   } else {[m
[32m+[m[32m  //     op0 = std::make_unique<ValueAST>(word0);[m
[32m+[m[32m  //   }[m
[32m+[m[32m  // } else {[m
[32m+[m[32m  //   assert(false && "Failed to parse expression\n");[m
[32m+[m[32m  // }[m
[32m+[m
   CheckTokenizer(tokenizer);[m
   auto [tok1, word1] = tokenizer.GetNextToken();[m
 [m
[36m@@ -93,6 +135,46 @@[m [mstd::unique_ptr<ExprAST> ParseExpression(Tokenizer &tokenizer,[m
   }[m
 }[m
 [m
[32m+[m[32mstd::unique_ptr<FunctionCallAST> ParseFunctionCall(Tokenizer &tokenizer,[m
[32m+[m[32m                                                   SymbolTable &st) {[m
[32m+[m[32m  CheckTokenizer(tokenizer);[m
[32m+[m[32m  auto [token0, word0] = tokenizer.GetNextToken();[m
[32m+[m[32m  assert(token0 == Text && "Expected function name.\n");[m
[32m+[m
[32m+[m[32m  if (st.CheckFunction(word0)) {[m
[32m+[m[32m    assert(false && "Calling undefined function.\n");[m
[32m+[m[32m  }[m
[32m+[m
[32m+[m[32m  CheckTokenizer(tokenizer);[m
[32m+[m[32m  auto [token1, word1] = tokenizer.GetNextToken();[m
[32m+[m[32m  assert(token1 == LBrack && "Expected (\n.");[m
[32m+[m
[32m+[m[32m  std::vector<std::unique_ptr<ExprAST>> args;[m
[32m+[m[32m  while (true) {[m
[32m+[m[32m    CheckTokenizer(tokenizer);[m
[32m+[m[32m    auto [token, word] = tokenizer.GetNextToken();[m
[32m+[m
[32m+[m[32m    if (token == RBrack) {[m
[32m+[m[32m      break;[m
[32m+[m[32m    }[m
[32m+[m
[32m+[m[32m    if (token == Comma) {[m
[32m+[m[32m      continue;[m
[32m+[m[32m    }[m
[32m+[m
[32m+[m[32m    if (std::isalpha(word[0])) {[m
[32m+[m[32m      assert(st.CheckSymbol(word) && "Undefined variable");[m
[32m+[m[32m      args.push_back(std::make_unique<VarAST>(word));[m
[32m+[m[32m    } else {[m
[32m+[m[32m      args.push_back(std::make_unique<ValueAST>(word));[m
[32m+[m[32m    }[m
[32m+[m[32m  }[m
[32m+[m
[32m+[m[32m  auto *prototype = st.GetPrototype(word0);[m
[32m+[m[32m  return std::make_unique<FunctionCallAST>(word0, std::move(args),[m
[32m+[m[32m                                           prototype->GetInstructions());[m
[32m+[m[32m}[m
[32m+[m
 std::unique_ptr<StatementAST> ParseStatement(Tokenizer &tokenizer,[m
                                              SymbolTable &st) {[m
   CheckTokenizer(tokenizer);[m
[1mdiff --git a/src/tokenizer.cpp b/src/tokenizer.cpp[m
[1mindex d3458eb..a2ab7ba 100644[m
[1m--- a/src/tokenizer.cpp[m
[1m+++ b/src/tokenizer.cpp[m
[36m@@ -42,11 +42,12 @@[m [mstatic const std::unordered_map<std::string, Token> token_map = {[m
     {"]", Token::RSBrack},[m
     {";", Token::SColon},[m
     {",", Token::Comma},[m
[31m-    {"fn", Token::Fn}};[m
[32m+[m[32m    {"fn", Token::Fn},[m
[32m+[m[32m    {"call", Token::Call}};[m
 [m
 static const std::unordered_set<char> stop_words = {[m
[31m-    ';', '(', ')', '{', '}', ' ', '[', ']', '+', '-', ',',[m
[31m-    '*', '/', '%', '=', '!', '<', '>', '&', '|'};[m
[32m+[m[32m    ';', '(', ')', '{', '}', ' ', '[', ']', '+', '-',[m
[32m+[m[32m    ',', '*', '/', '%', '=', '!', '<', '>', '&', '|'};[m
 [m
 // TODO: Fix for case && and ||[m
 static const std::unordered_set<char> sp_words = {'=', '!', '>', '<', '+', '-'};[m
