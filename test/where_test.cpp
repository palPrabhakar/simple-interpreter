#include <gtest/gtest.h>

#include "interpreter.h"
#include "parser.h"
#include "symbol_table.h"
#include "tokenizer.h"

TEST(Where_Test, TestSimpleWhere) {
  auto st = tci::SymbolTable();
  auto interp = tci::Interpreter(st);
  st.InsertSymbol("x");
  st.SetValue("x", 1);

  auto tokenizer = tci::Tokenizer("while (x < 10) { mut x = x + 1; }");
  auto ast = tci::Parse(tokenizer, st);
  uint ridx = 1;
  auto code = ast->GenerateCode(ridx);

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

  interp.Interpret(std::move(code));
  EXPECT_TRUE(st.CheckSymbol("x"));
  EXPECT_EQ(st.GetValue("x"), 10);
}

TEST(Where_Test, TestWhereWithIf) {
  auto st = tci::SymbolTable();
  auto interp = tci::Interpreter(st);
  st.InsertSymbol("x");
  st.SetValue("x", 1);

  st.InsertSymbol("i");
  st.SetValue("i", 0);

  auto tokenizer = tci::Tokenizer(
      "while (i < 10) { if(i < 5) { mut x = x*2; } else { mut x = x+1; } mut i "
      "= i + 1; }");
  auto ast = tci::Parse(tokenizer, st);
  uint ridx = 1;
  auto code = ast->GenerateCode(ridx);

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

  interp.Interpret(std::move(code));
  EXPECT_TRUE(st.CheckSymbol("x"));
  EXPECT_EQ(st.GetValue("x"), 37);
  EXPECT_TRUE(st.CheckSymbol("i"));
  EXPECT_EQ(st.GetValue("i"), 10);
}
