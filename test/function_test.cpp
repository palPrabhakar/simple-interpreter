#include "interpreter_fixture.hpp"

TEST_F(InterpreterTest, TestSimpleFunction) {
    SetCode("fn double(x) { return x*2; }");
    SetCode("let a = call double(1);");
    EXPECT_EQ(interp.GetRegValue(st.GetReg("a")), 2);
}

TEST_F(InterpreterTest, TestSimpleFunctionST) {
    SetCode("let x = 2;");
    SetCode("fn double(x) { return x*2; }");
    SetCode("let a = call double(1);");
    EXPECT_EQ(interp.GetRegValue(st.GetReg("a")), 2);
}

TEST_F(InterpreterTest, TestTwoArgFunction) {
    SetCode("fn add(x, y) { return x + y; }");
    SetCode("let a = call add(1, 2);");
    EXPECT_EQ(interp.GetRegValue(st.GetReg("a")), 3);
}

TEST_F(InterpreterTest, TestNestedFunction) {
    SetCode("fn double(x) { return x*2; }");
    SetCode("fn triple(x) { return call double(x) + x; }");
    SetCode("let a = call triple(1);");
    EXPECT_EQ(interp.GetRegValue(st.GetReg("a")), 3);
}

TEST_F(InterpreterTest, TestNestedTwoArgFunction) {
    SetCode("fn double(x) { return x*2; }");
    SetCode(
        "fn add_doubles(x, y) { let xd = call double(x); let yd = call "
        "double(y); return xd + yd; }");

    SetCode("let a = call add_doubles(1, 2);");
    EXPECT_EQ(interp.GetRegValue(st.GetReg("a")), 6);

    SetCode("let b = call add_doubles(2, 4);");
    EXPECT_EQ(interp.GetRegValue(st.GetReg("b")), 12);

    SetCode("let c = call add_doubles(a, b);");
    EXPECT_EQ(interp.GetRegValue(st.GetReg("c")), 36);
}

TEST_F(InterpreterTest, FactorialFunction) {
    // clang-format off
 SetCode("fn factorial(x) {"
                            " if (x < 0) {"
                              "mut x = 0; "
                            "} else {"
                              "let c = x-1;"
                              "while(c > 0) {"
                                "mut x = x * c;"
                                "mut c = c - 1;"
                              "}"
                            "}"
                          "return x;"
                         "}");
    // clang-format on
    SetCode("let f = call factorial(0);");
    EXPECT_EQ(interp.GetRegValue(st.GetReg("f")), 0);

    SetCode("mut f = call factorial(1);");
    EXPECT_EQ(interp.GetRegValue(st.GetReg("f")), 1);

    SetCode("mut f = call factorial(2);");
    EXPECT_EQ(interp.GetRegValue(st.GetReg("f")), 2);

    SetCode("mut f = call factorial(3);");
    EXPECT_EQ(interp.GetRegValue(st.GetReg("f")), 6);

    SetCode("mut f = call factorial(4);");
    EXPECT_EQ(interp.GetRegValue(st.GetReg("f")), 24);

    SetCode("mut f = call factorial(5);");
    EXPECT_EQ(interp.GetRegValue(st.GetReg("f")), 120);
}

TEST_F(InterpreterTest, TestLargeFunction) {
    SetCode(
        "fn Big_Fn(x) {"
        "let y = x*2;"
        "let z = y+x;"
        "let a = x+y*z;"
        "if(z < x*4) {"
        "mut z = z*x*4;"
        "} else {"
        "mut y = z;"
        "mut z = z/4;"
        "}"
        "let b = a*z;"
        "while (b > 0) {"
        "mut b = b-1;"
        "}"
        "return 1;"
        "}");

    SetCode("let a = call Big_Fn(1);");
    EXPECT_EQ(interp.GetRegValue(st.GetReg("a")), 1);
}
