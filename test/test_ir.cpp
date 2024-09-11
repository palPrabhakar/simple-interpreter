#include <gtest/gtest.h>

#include "parser.h"
#include "symbol_table.h"
#include "tokenizer.h"

TEST(IR_Test, TestExprIR) {
  auto st = sci::SymbolTable();
  uint ridx = 1;

  auto tokenizer = sci::Tokenizer("let x  = 1;");
  auto ast = sci::Parse(tokenizer, st);
  auto code = ast->GenerateCode(ridx);
  EXPECT_EQ(code.size(), 2);
  EXPECT_EQ(code[0].ToString(), "loadi 1 r1");
  EXPECT_EQ(code[1].ToString(), "store r1 m@x");

  tokenizer.ResetTokenizer("let y = x*5;");
  ast = sci::Parse(tokenizer, st);
  code = ast->GenerateCode(ridx=1);
  EXPECT_EQ(code.size(), 4);
  EXPECT_EQ(code[0].ToString(), "load m@x r1");
  EXPECT_EQ(code[1].ToString(), "loadi 5 r2");
  EXPECT_EQ(code[2].ToString(), "mul r1 r2 r3");
  EXPECT_EQ(code[3].ToString(), "store r3 m@y");

  tokenizer.ResetTokenizer("let z = x+y - 5;");
  ast = sci::Parse(tokenizer, st);
  code = ast->GenerateCode(ridx=1);
  EXPECT_EQ(code.size(), 6);
  EXPECT_EQ(code[0].ToString(), "load m@x r1");
  EXPECT_EQ(code[1].ToString(), "load m@y r2");
  EXPECT_EQ(code[2].ToString(), "add r1 r2 r3");
  EXPECT_EQ(code[3].ToString(), "loadi 5 r4");
  EXPECT_EQ(code[4].ToString(), "sub r3 r4 r5");
  EXPECT_EQ(code[5].ToString(), "store r5 m@z");

  tokenizer.ResetTokenizer("mut x = 5;");
  ast = sci::Parse(tokenizer, st);
  code = ast->GenerateCode(ridx=1);
  EXPECT_EQ(code.size(), 2);
  EXPECT_EQ(code[0].ToString(), "loadi 5 r1");
  EXPECT_EQ(code[1].ToString(), "store r1 m@x");

  tokenizer.ResetTokenizer("mut y = x/5;");
  ast = sci::Parse(tokenizer, st);
  code = ast->GenerateCode(ridx=1);
  EXPECT_EQ(code.size(), 4);
  EXPECT_EQ(code[0].ToString(), "load m@x r1");
  EXPECT_EQ(code[1].ToString(), "loadi 5 r2");
  EXPECT_EQ(code[2].ToString(), "div r1 r2 r3");
  EXPECT_EQ(code[3].ToString(), "store r3 m@y");
}

TEST(IR_Loop_Test, TestSimpleLoopIR) {
  auto st = sci::SymbolTable();
  uint ridx = 1;

  st.InsertSymbol("x");
  st.SetValue("x", 1);
  auto tokenizer = sci::Tokenizer("while (x < 10) { mut x = x + 1; }");
  auto ast = sci::Parse(tokenizer, st);
  auto code = ast->GenerateCode(ridx=1);
  EXPECT_EQ(code.size(), 10);
  EXPECT_EQ(code[0].ToString(), "load m@x r1");
  EXPECT_EQ(code[1].ToString(), "loadi 10 r2");
  EXPECT_EQ(code[2].ToString(), "lt r1 r2 r3");
  EXPECT_EQ(code[3].ToString(), "cjmp r3 p@2");
  EXPECT_EQ(code[4].ToString(), "jmp p@6");
  EXPECT_EQ(code[5].ToString(), "load m@x r4");
  EXPECT_EQ(code[6].ToString(), "loadi 1 r5");
  EXPECT_EQ(code[7].ToString(), "add r4 r5 r6");
  EXPECT_EQ(code[8].ToString(), "store r6 m@x");
  EXPECT_EQ(code[9].ToString(), "jmp p@-9");
}

TEST(IR_Loop_Test, TestLoopWithCondIR) {
  auto st = sci::SymbolTable();
  uint ridx = 1;

  st.InsertSymbol("x");
  st.SetValue("x", 1);
  st.InsertSymbol("i");
  st.SetValue("i", 0);

  auto tokenizer = sci::Tokenizer(
      "while (i < 10) { if(i < 5) { mut x = x*2; } else { mut x = x+1; } mut i "
      "= i + 1; }");
  auto ast = sci::Parse(tokenizer, st);
  auto code = ast->GenerateCode(ridx=1);

  EXPECT_EQ(code.size(), 23);
  EXPECT_EQ(code[0].ToString(), "load m@i r1");
  EXPECT_EQ(code[1].ToString(), "loadi 10 r2");
  EXPECT_EQ(code[2].ToString(), "lt r1 r2 r3");
  EXPECT_EQ(code[3].ToString(), "cjmp r3 p@2");
  EXPECT_EQ(code[4].ToString(), "jmp p@19");
  EXPECT_EQ(code[5].ToString(), "load m@i r4");
  EXPECT_EQ(code[6].ToString(), "loadi 5 r5");
  EXPECT_EQ(code[7].ToString(), "lt r4 r5 r6");
  EXPECT_EQ(code[8].ToString(), "cjmp r6 p@6");
  EXPECT_EQ(code[9].ToString(), "load m@x r7");  // follow-through else branch
  EXPECT_EQ(code[10].ToString(), "loadi 1 r8");
  EXPECT_EQ(code[11].ToString(), "add r7 r8 r9");
  EXPECT_EQ(code[12].ToString(), "store r9 m@x");
  EXPECT_EQ(code[13].ToString(), "jmp p@5");        // jump past true branch
  EXPECT_EQ(code[14].ToString(), "load m@x r10");  // true branch
  EXPECT_EQ(code[15].ToString(), "loadi 2 r11");
  EXPECT_EQ(code[16].ToString(), "mul r10 r11 r12");
  EXPECT_EQ(code[17].ToString(), "store r12 m@x");
  EXPECT_EQ(code[18].ToString(), "load m@i r13");  //
  EXPECT_EQ(code[19].ToString(), "loadi 1 r14");   //
  EXPECT_EQ(code[20].ToString(), "add r13 r14 r15");
  EXPECT_EQ(code[21].ToString(), "store r15 m@i");
  EXPECT_EQ(code[22].ToString(), "jmp p@-22");  //
}

TEST(IR_Loop_Test, DISABLED_TestLoopWithCondAndBreakIR) {
  auto st = sci::SymbolTable();
  st.InsertSymbol("x");
  st.SetValue("x", 1);

  st.InsertSymbol("i");
  st.SetValue("i", 0);

  auto tokenizer = sci::Tokenizer(
      "while (i < 10) { if(i < 5) { mut x = x*2; if(x>5){break;}else{mut x=x+2;} } else { mut x = x+1; } mut i "
      "= i + 1; }");
  auto ast = sci::Parse(tokenizer, st);
  uint ridx = 1;
  auto code = ast->GenerateCode(ridx);

  // EXPECT_EQ(code.size(), 23);
  // EXPECT_EQ(code[0].ToString(), "load m@i r1");
  // EXPECT_EQ(code[1].ToString(), "loadi 10 r2");
  // EXPECT_EQ(code[2].ToString(), "lt r1 r2 r3");
  // EXPECT_EQ(code[3].ToString(), "cjmp r3 p@2");
  // EXPECT_EQ(code[4].ToString(), "jmp p@19"); // jump outside loop
  // EXPECT_EQ(code[5].ToString(), "load m@i r4"); // if case
  // EXPECT_EQ(code[6].ToString(), "loadi 5 r5");
  // EXPECT_EQ(code[7].ToString(), "lt r4 r5 r6");
  // EXPECT_EQ(code[8].ToString(), "cjmp r6 p@6"); 
  // EXPECT_EQ(code[9].ToString(), "load m@x r7");  // follow-through else branch
  // EXPECT_EQ(code[10].ToString(), "loadi 1 r8");
  // EXPECT_EQ(code[11].ToString(), "add r7 r8 r9");
  // EXPECT_EQ(code[12].ToString(), "store r9 m@x");
  // EXPECT_EQ(code[13].ToString(), "jmp p@5");        // jump past true branch
  // EXPECT_EQ(code[14].ToString(), "load m@x r10");  // true branch
  // EXPECT_EQ(code[15].ToString(), "loadi 2 r11");
  // EXPECT_EQ(code[16].ToString(), "mul r10 r11 r12");
  // EXPECT_EQ(code[17].ToString(), "store r12 m@x");
  // EXPECT_EQ(code[18].ToString(), "load m@i r13");  //
  // EXPECT_EQ(code[19].ToString(), "loadi 1 r14");   //
  // EXPECT_EQ(code[20].ToString(), "add r13 r14 r15");
  // EXPECT_EQ(code[21].ToString(), "store r15 m@i");
  // EXPECT_EQ(code[22].ToString(), "jmp p@-22");  //
}

TEST(IR_Loop_Test, TestLoopWithBreakIR) {
  auto st = sci::SymbolTable();
  uint ridx = 1;

  st.InsertSymbol("x");
  st.SetValue("x", 2);
  st.InsertSymbol("i");
  st.SetValue("i", 0);

  auto tokenizer = sci::Tokenizer(
      "while (i < 10) { mut i = i + 1; mut x = x + i * x; break; }");
  auto ast = sci::Parse(tokenizer, st);
  auto code = ast->GenerateCode(ridx=1);

  EXPECT_EQ(code.size(), 17);
  EXPECT_EQ(code[0].ToString(), "load m@i r1");
  EXPECT_EQ(code[1].ToString(), "loadi 10 r2");
  EXPECT_EQ(code[2].ToString(), "lt r1 r2 r3");
  EXPECT_EQ(code[3].ToString(), "cjmp r3 p@2");
  EXPECT_EQ(code[4].ToString(), "jmp p@13");
  EXPECT_EQ(code[5].ToString(), "load m@i r4");
  EXPECT_EQ(code[6].ToString(), "loadi 1 r5");
  EXPECT_EQ(code[7].ToString(), "add r4 r5 r6");
  EXPECT_EQ(code[8].ToString(), "store r6 m@i");
  EXPECT_EQ(code[9].ToString(), "load m@x r7");
  EXPECT_EQ(code[10].ToString(), "load m@i r8");
  EXPECT_EQ(code[11].ToString(), "load m@x r9");
  EXPECT_EQ(code[12].ToString(), "mul r8 r9 r10");
  EXPECT_EQ(code[13].ToString(), "add r7 r10 r11");
  EXPECT_EQ(code[14].ToString(), "store r11 m@x");
  EXPECT_EQ(code[15].ToString(), "jmp p@2");
  EXPECT_EQ(code[16].ToString(), "jmp p@-16");
}

TEST(IR_Loop_Test, TestLoopWithContinueIR) {
  auto st = sci::SymbolTable();
  uint ridx = 1;

  st.InsertSymbol("x");
  st.SetValue("x", 2);
  st.InsertSymbol("i");
  st.SetValue("i", 0);

  auto tokenizer = sci::Tokenizer(
      "while (i < 10) { mut i = i + 1; continue; mut x = x + i * x; }");
  auto ast = sci::Parse(tokenizer, st);
  auto code = ast->GenerateCode(ridx);

  EXPECT_EQ(code.size(), 17);
  EXPECT_EQ(code[0].ToString(), "load m@i r1");
  EXPECT_EQ(code[1].ToString(), "loadi 10 r2");
  EXPECT_EQ(code[2].ToString(), "lt r1 r2 r3");
  EXPECT_EQ(code[3].ToString(), "cjmp r3 p@2");
  EXPECT_EQ(code[4].ToString(), "jmp p@13");
  EXPECT_EQ(code[5].ToString(), "load m@i r4");
  EXPECT_EQ(code[6].ToString(), "loadi 1 r5");
  EXPECT_EQ(code[7].ToString(), "add r4 r5 r6");
  EXPECT_EQ(code[8].ToString(), "store r6 m@i"); // mut i = i + 1; end 
  EXPECT_EQ(code[9].ToString(), "jmp p@-9");
  EXPECT_EQ(code[10].ToString(), "load m@x r7");
  EXPECT_EQ(code[11].ToString(), "load m@i r8");
  EXPECT_EQ(code[12].ToString(), "load m@x r9");
  EXPECT_EQ(code[13].ToString(), "mul r8 r9 r10");
  EXPECT_EQ(code[14].ToString(), "add r7 r10 r11");
  EXPECT_EQ(code[15].ToString(), "store r11 m@x");
  EXPECT_EQ(code[16].ToString(), "jmp p@-16");
}

TEST(IR_IfElse_Test, TestIfElseStatementIR) {
  auto st = sci::SymbolTable();
  uint ridx = 1;

  st.InsertSymbol("x");
  st.SetValue("x", 1);

  auto tokenizer =
      sci::Tokenizer("if (x  < 10) { mut x = x * 4; } else { mut x = x / 2; }");
  auto ast = sci::Parse(tokenizer, st);
  auto code = ast->GenerateCode(ridx);

  EXPECT_EQ(code.size(), 13);
  EXPECT_EQ(code[0].ToString(), "load m@x r1");
  EXPECT_EQ(code[1].ToString(), "loadi 10 r2");
  EXPECT_EQ(code[2].ToString(), "lt r1 r2 r3");
  EXPECT_EQ(code[3].ToString(), "cjmp r3 p@6");
  EXPECT_EQ(code[4].ToString(), "load m@x r4");
  EXPECT_EQ(code[5].ToString(), "loadi 2 r5");
  EXPECT_EQ(code[6].ToString(), "div r4 r5 r6");
  EXPECT_EQ(code[7].ToString(), "store r6 m@x");
  EXPECT_EQ(code[8].ToString(), "jmp p@5");
  EXPECT_EQ(code[9].ToString(), "load m@x r7");
  EXPECT_EQ(code[10].ToString(), "loadi 4 r8");
  EXPECT_EQ(code[11].ToString(), "mul r7 r8 r9");
  EXPECT_EQ(code[12].ToString(), "store r9 m@x");
}

TEST(IR_IfElse_Test, TestNestedIfElseStatementIR) {
  auto st = sci::SymbolTable();
  uint ridx = 1;

  st.InsertSymbol("x");
  st.SetValue("x", 4);
  st.InsertSymbol("y");
  st.SetValue("y", 5);

  auto tokenizer = sci::Tokenizer(
      "if (x<10) { if (y>10) { mut x = y/x; } else { mut x = x*y; } } else { "
      "mut "
      "x=x*4; }");
  auto ast = sci::Parse(tokenizer, st);
  auto code = ast->GenerateCode(ridx);

  EXPECT_EQ(code.size(), 22);
  EXPECT_EQ(code[0].ToString(), "load m@x r1");
  EXPECT_EQ(code[1].ToString(), "loadi 10 r2");
  EXPECT_EQ(code[2].ToString(), "lt r1 r2 r3");
  EXPECT_EQ(code[3].ToString(), "cjmp r3 p@6");
  EXPECT_EQ(code[4].ToString(), "load m@x r4");
  EXPECT_EQ(code[5].ToString(), "loadi 4 r5");
  EXPECT_EQ(code[6].ToString(), "mul r4 r5 r6");
  EXPECT_EQ(code[7].ToString(), "store r6 m@x");
  EXPECT_EQ(code[8].ToString(), "jmp p@14");
  EXPECT_EQ(code[9].ToString(), "load m@y r7");
  EXPECT_EQ(code[10].ToString(), "loadi 10 r8");
  EXPECT_EQ(code[11].ToString(), "gt r7 r8 r9");
  EXPECT_EQ(code[12].ToString(), "cjmp r9 p@6");
  EXPECT_EQ(code[13].ToString(), "load m@x r10");
  EXPECT_EQ(code[14].ToString(), "load m@y r11");
  EXPECT_EQ(code[15].ToString(), "mul r10 r11 r12");
  EXPECT_EQ(code[16].ToString(), "store r12 m@x");
  EXPECT_EQ(code[17].ToString(), "jmp p@5");
  EXPECT_EQ(code[18].ToString(), "load m@y r13");
  EXPECT_EQ(code[19].ToString(), "load m@x r14");
  EXPECT_EQ(code[20].ToString(), "div r13 r14 r15");
  EXPECT_EQ(code[21].ToString(), "store r15 m@x");
}

TEST(IR_Function_Test, DISABLED_TestSimpleFunctionIR) {
  auto st = sci::SymbolTable();
  uint ridx = 1;

  auto tokenizer = sci::Tokenizer("fn double(x) { return x*2; }");
  auto ast = sci::Parse(tokenizer, st);
  auto code = ast->GenerateCode(ridx=1);

  tokenizer.ResetTokenizer("let a = call double(1);");
  ast = sci::Parse(tokenizer, st);
  code = ast->GenerateCode(ridx=1);

  EXPECT_EQ(code.size(), 8);
  EXPECT_EQ(code[0].ToString(), "loadi 1 r1");
  EXPECT_EQ(code[1].ToString(), "call double");
  EXPECT_EQ(code[2].ToString(), "store r1 m@x");
  EXPECT_EQ(code[3].ToString(), "load m@x r2");
  EXPECT_EQ(code[4].ToString(), "loadi 2 r3");
  EXPECT_EQ(code[5].ToString(), "mul r2 r3 r4");
  EXPECT_EQ(code[6].ToString(), "ret");
  EXPECT_EQ(code[7].ToString(), "store r4 m@a");
}

TEST(IR_Function_Test, DISABLED_TestTwoArgFunctionIR) {
  auto st = sci::SymbolTable();
  uint ridx = 1;

  auto tokenizer = sci::Tokenizer("fn add(x, y) { return x + y; }");
  auto ast = sci::Parse(tokenizer, st);
  auto code = ast->GenerateCode(ridx=1);

  tokenizer.ResetTokenizer("let a = call add(1, 2);");
  ast = sci::Parse(tokenizer, st);
  code = ast->GenerateCode(ridx=1);

  EXPECT_EQ(code.size(), 10);
  EXPECT_EQ(code[0].ToString(), "loadi 1 r1");
  EXPECT_EQ(code[1].ToString(), "loadi 2 r2");
  EXPECT_EQ(code[2].ToString(), "call add");
  EXPECT_EQ(code[3].ToString(), "store r1 m@x");
  EXPECT_EQ(code[4].ToString(), "store r2 m@y");
  EXPECT_EQ(code[5].ToString(), "load m@x r3");
  EXPECT_EQ(code[6].ToString(), "load m@y r4");
  EXPECT_EQ(code[7].ToString(), "add r3 r4 r5");
  EXPECT_EQ(code[8].ToString(), "ret");
  EXPECT_EQ(code[9].ToString(), "store r5 m@a");
}

TEST(IR_Function_Test, DISABLED_TestNestedFunctionIR) {
  auto st = sci::SymbolTable();
  uint ridx = 1;

  auto tokenizer = sci::Tokenizer("fn double(x) { return x*2; }");
  auto ast = sci::Parse(tokenizer, st);
  auto code = ast->GenerateCode(ridx=1);

  tokenizer.ResetTokenizer("fn triple(x) { return call double(x) + x; }");
  ast = sci::Parse(tokenizer, st);
  code = ast->GenerateCode(ridx=1);

  tokenizer.ResetTokenizer("let a = call triple(1);");
  ast = sci::Parse(tokenizer, st);
  code = ast->GenerateCode(ridx=1);

  EXPECT_EQ(code.size(), 14);
  EXPECT_EQ(code[0].ToString(), "loadi 1 r1");
  EXPECT_EQ(code[1].ToString(), "call triple");
  EXPECT_EQ(code[2].ToString(), "store r1 m@x");
  EXPECT_EQ(code[3].ToString(), "load m@x r2");
  EXPECT_EQ(code[4].ToString(), "call double");
  EXPECT_EQ(code[5].ToString(), "store r2 m@x");
  EXPECT_EQ(code[6].ToString(), "load m@x r3");
  EXPECT_EQ(code[7].ToString(), "loadi 2 r4");
  EXPECT_EQ(code[8].ToString(), "mul r3 r4 r5");
  EXPECT_EQ(code[9].ToString(), "ret");
  EXPECT_EQ(code[10].ToString(), "load m@x r6");
  EXPECT_EQ(code[11].ToString(), "add r5 r6 r7");
  EXPECT_EQ(code[12].ToString(), "ret");
  EXPECT_EQ(code[13].ToString(), "store r7 m@a");
}

