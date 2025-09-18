#include "interpreter_fixture.hpp"

TEST_F(InterpreterTest, TestSimpleLoop) {
    SetCode("let x = 1;");
    SetCode("while (x < 10) { mut x = x + 1; }");

    EXPECT_EQ(interp.GetRegValue(st.GetReg("x")), 10);
}

TEST_F(InterpreterTest, TestLoopWithCond) {
    SetCode("let x = 1;");
    SetCode("let i = 0;");
    SetCode(
        "while (i < 10) { if(i < 5) { mut x = x*2; } else { mut x = x+1; } mut "
        "i "
        "= i + 1; }");

    EXPECT_EQ(interp.GetRegValue(st.GetReg("x")), 37);
    EXPECT_EQ(interp.GetRegValue(st.GetReg("i")), 10);
}

TEST_F(InterpreterTest, TestLoopWithCondAndBreak) {
    SetCode("let x = 1;");
    SetCode("let i = 0;");

    SetCode(
        "while (i < 10) { if(i < 5) { mut x = x*2; if(x>5){break;}else{mut "
        "x=x+2;} } else { mut x = x+1; } mut i "
        "= i + 1; }");

    EXPECT_EQ(interp.GetRegValue(st.GetReg("x")), 8);
    EXPECT_EQ(interp.GetRegValue(st.GetReg("i")), 1);
}

TEST_F(InterpreterTest, TestLoopWithIfAndContinue) {
    SetCode("let x = 1;");
    SetCode("let i = 0;");

    SetCode(
        "while (i < 10) { mut i = i + 1; if(i < 5) { continue; mut x = x*2; } "
        "else { mut x = x+1; }}");

    EXPECT_EQ(interp.GetRegValue(st.GetReg("x")), 7);
    EXPECT_EQ(interp.GetRegValue(st.GetReg("i")), 10);
}

TEST_F(InterpreterTest, TestLoopWithBreak) {
    SetCode("let x = 2;");
    SetCode("let i = 0;");

    SetCode("while (i < 10) { mut i = i + 1; mut x = x + i * x; break; }");

    EXPECT_EQ(interp.GetRegValue(st.GetReg("x")), 4);
    EXPECT_EQ(interp.GetRegValue(st.GetReg("i")), 1);
}

TEST_F(InterpreterTest, TestLoopWithContinue) {
    SetCode("let x = 2;");
    SetCode("let i = 0;");

    SetCode("while (i < 10) { mut i = i + 1; continue; mut x = x + i * x; }");

    EXPECT_EQ(interp.GetRegValue(st.GetReg("x")), 2);
    EXPECT_EQ(interp.GetRegValue(st.GetReg("i")), 10);
}
