#include <gtest/gtest.h>

#include "instructions.h"
#include "parser.h"
#include "symbol_table.h"
#include "tokenizer.h"

TEST(IR_Test, TestExprIR) {
    auto st = sci::SymbolTable();

    auto tokenizer = sci::Tokenizer("let x = 1;");
    auto ast = sci::Parse(tokenizer, st);
    auto code = ast->GenerateCode(st);
    EXPECT_EQ(code.size(), 1);
    EXPECT_EQ(code[0].ToString(), "loadi 1 r1");

    tokenizer.ResetTokenizer("let y = x*5;");
    ast = sci::Parse(tokenizer, st);
    code = ast->GenerateCode(st);
    EXPECT_EQ(code.size(), 2);
    EXPECT_EQ(code[0].ToString(), "loadi 5 r2");
    EXPECT_EQ(code[1].ToString(), "mul r1 r2 r3");

    tokenizer.ResetTokenizer("let z = x+y - 5;");
    ast = sci::Parse(tokenizer, st);
    code = ast->GenerateCode(st);
    EXPECT_EQ(code.size(), 2);
    EXPECT_EQ(code[0].ToString(), "add r1 r3 r4");
    EXPECT_EQ(code[1].ToString(), "sub r4 r2 r5");

    tokenizer.ResetTokenizer("mut x = 5;");
    ast = sci::Parse(tokenizer, st);
    code = ast->GenerateCode(st);
    EXPECT_EQ(code.size(), 1);
    EXPECT_EQ(code[0].ToString(), "rmov r2 r1");

    tokenizer.ResetTokenizer("mut y = z;");
    ast = sci::Parse(tokenizer, st);
    code = ast->GenerateCode(st);
    EXPECT_EQ(code.size(), 1);
    EXPECT_EQ(code[0].ToString(), "rmov r5 r3");

    tokenizer.ResetTokenizer("mut y = x/5;");
    ast = sci::Parse(tokenizer, st);
    code = ast->GenerateCode(st);
    EXPECT_EQ(code.size(), 1);
    EXPECT_EQ(code[0].ToString(), "div r1 r2 r3");
}

TEST(IR_Test, TestSimpleLoopIR) {
    auto st = sci::SymbolTable();

    auto tokenizer = sci::Tokenizer("let x = 1;");
    auto ast = sci::Parse(tokenizer, st);
    auto code = ast->GenerateCode(st);
    EXPECT_EQ(code.size(), 1);
    EXPECT_EQ(code[0].ToString(), "loadi 1 r1");

    tokenizer.ResetTokenizer("while (x < 10) { mut x = x + 1; }");
    ast = sci::Parse(tokenizer, st);
    code = ast->GenerateCode(st);
    EXPECT_EQ(code.size(), 7);
    EXPECT_EQ(code[0].ToString(), "loadi 10 r2");
    EXPECT_EQ(code[1].ToString(), "lt r1 r2 r3");
    EXPECT_EQ(code[2].ToString(), "cjmp r3 p@2");
    EXPECT_EQ(code[3].ToString(), "jmp p@4");
    EXPECT_EQ(code[4].ToString(), "loadi 1 r4");
    EXPECT_EQ(code[5].ToString(), "add r1 r4 r1");
    EXPECT_EQ(code[6].ToString(), "jmp p@-5");
}

TEST(IR_Test, TestLoopWithCondIR) {
    auto st = sci::SymbolTable();

    auto tokenizer = sci::Tokenizer("let x = 1;");
    auto ast = sci::Parse(tokenizer, st);
    auto code = ast->GenerateCode(st);

    tokenizer.ResetTokenizer("let i = 1;");
    ast = sci::Parse(tokenizer, st);
    code = ast->GenerateCode(st);

    tokenizer.ResetTokenizer(
        "while (i < 10) { if(i < 5) { break; } else { mut x = x+1; } mut "
        "i "
        "= i + 1; }");
    ast = sci::Parse(tokenizer, st);
    code = ast->GenerateCode(st);

    EXPECT_EQ(code.size(), 13);
    EXPECT_EQ(code[0].ToString(), "loadi 10 r3");
    EXPECT_EQ(code[1].ToString(), "lt r2 r3 r4");
    EXPECT_EQ(code[2].ToString(), "cjmp r4 p@2");
    EXPECT_EQ(code[3].ToString(), "jmp p@10"); // end of loop
    EXPECT_EQ(code[4].ToString(), "loadi 5 r5");
    EXPECT_EQ(code[5].ToString(), "lt r2 r5 r6");
    EXPECT_EQ(code[6].ToString(), "cjmp r6 p@4");
    EXPECT_EQ(code[7].ToString(), "loadi 1 r7");
    EXPECT_EQ(code[8].ToString(), "add r1 r7 r1");
    EXPECT_EQ(code[9].ToString(), "jmp p@2");       // jump past true branch
    EXPECT_EQ(code[10].ToString(), "jmp p@3");  // true branch -- jmp end of loop
    EXPECT_EQ(code[11].ToString(), "add r2 r7 r2");
    EXPECT_EQ(code[12].ToString(), "jmp p@-11");  //
}

TEST(IR_Test, TestIfElseStatementIR) {
    auto st = sci::SymbolTable();

    auto tokenizer = sci::Tokenizer("let x = 1;");
    auto ast = sci::Parse(tokenizer, st);
    auto code = ast->GenerateCode(st);
    EXPECT_EQ(code.size(), 1);
    EXPECT_EQ(code[0].ToString(), "loadi 1 r1");

    tokenizer.ResetTokenizer(
        "if (x  < 10) { mut x = x * 4; } else { mut x = x / 2; }");
    ast = sci::Parse(tokenizer, st);
    code = ast->GenerateCode(st);

    EXPECT_EQ(code.size(), 8);
    EXPECT_EQ(code[0].ToString(), "loadi 10 r2");
    EXPECT_EQ(code[1].ToString(), "lt r1 r2 r3");
    EXPECT_EQ(code[2].ToString(), "cjmp r3 p@4");
    EXPECT_EQ(code[3].ToString(), "loadi 2 r4");
    EXPECT_EQ(code[4].ToString(), "div r1 r4 r1");
    EXPECT_EQ(code[5].ToString(), "jmp p@3");
    EXPECT_EQ(code[6].ToString(), "loadi 4 r5");
    EXPECT_EQ(code[7].ToString(), "mul r1 r5 r1");
}

TEST(IR_Test, TestNestedIfElseStatementIR) {
    auto st = sci::SymbolTable();

    auto tokenizer = sci::Tokenizer("let x = 4;");
    auto ast = sci::Parse(tokenizer, st);
    auto code = ast->GenerateCode(st);
    EXPECT_EQ(code.size(), 1);
    EXPECT_EQ(code[0].ToString(), "loadi 4 r1");

    tokenizer.ResetTokenizer("let y = 5;");
    ast = sci::Parse(tokenizer, st);
    code = ast->GenerateCode(st);
    EXPECT_EQ(code.size(), 1);
    EXPECT_EQ(code[0].ToString(), "loadi 5 r2");

    tokenizer.ResetTokenizer(
        "if (x<10) { if (y>10) { mut x = y/x; } else { mut x = x*y; } } else { "
        "mut "
        "x=x*4; }");
    ast = sci::Parse(tokenizer, st);
    code = ast->GenerateCode(st);

    EXPECT_EQ(code.size(), 11);
    EXPECT_EQ(code[0].ToString(), "loadi 10 r3");
    EXPECT_EQ(code[1].ToString(), "lt r1 r3 r4");
    EXPECT_EQ(code[2].ToString(), "cjmp r4 p@4");
    EXPECT_EQ(code[3].ToString(), "loadi 4 r5");
    EXPECT_EQ(code[4].ToString(), "mul r1 r5 r1");
    EXPECT_EQ(code[5].ToString(), "jmp p@6");  // end
    EXPECT_EQ(code[6].ToString(), "gt r2 r3 r6");
    EXPECT_EQ(code[7].ToString(), "cjmp r6 p@3");
    EXPECT_EQ(code[8].ToString(), "mul r1 r2 r1");
    EXPECT_EQ(code[9].ToString(), "jmp p@2");  // end
    EXPECT_EQ(code[10].ToString(), "div r2 r1 r1");
}

TEST(IR_Test, DISABLED_TestSimpleFunctionIR) {
    auto st = sci::SymbolTable();

    auto tokenizer = sci::Tokenizer("fn double(x) { return x*2; }");
    auto ast = sci::Parse(tokenizer, st);
    auto code = ast->GenerateCode(st);

    tokenizer.ResetTokenizer("let a = call double(1);");
    ast = sci::Parse(tokenizer, st);
    code = ast->GenerateCode(st);

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

TEST(IR_Test, DISABLED_TestTwoArgFunctionIR) {
    auto st = sci::SymbolTable();

    auto tokenizer = sci::Tokenizer("fn add(x, y) { return x + y; }");
    auto ast = sci::Parse(tokenizer, st);
    auto code = ast->GenerateCode(st);

    tokenizer.ResetTokenizer("let a = call add(1, 2);");
    ast = sci::Parse(tokenizer, st);
    code = ast->GenerateCode(st);

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

TEST(IR_Test, DISABLED_TestNestedFunctionIR) {
    auto st = sci::SymbolTable();

    auto tokenizer = sci::Tokenizer("fn double(x) { return x*2; }");
    auto ast = sci::Parse(tokenizer, st);
    auto code = ast->GenerateCode(st);

    tokenizer.ResetTokenizer("fn triple(x) { return call double(x) + x; }");
    ast = sci::Parse(tokenizer, st);
    code = ast->GenerateCode(st);

    tokenizer.ResetTokenizer("let a = call triple(1);");
    ast = sci::Parse(tokenizer, st);
    code = ast->GenerateCode(st);

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
