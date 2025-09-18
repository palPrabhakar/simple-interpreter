#include "expr.h"

#include <iterator>
#include <string>

#include "instructions.h"
#include "symbol_table.h"

namespace sci {

std::vector<Instruction> ValueAST::GenerateCode(SymbolTable &st) {
    // ValueAST::GenerateCode is only called by OpAST
    // When used directly in StatementAST only the literal
    // value is read
    std::vector<Instruction> instructions;
    m_reg = st.GetReg(m_val);
    // load the immed val if not already present in some reg
    if (m_reg == -1) {
        m_reg = st.GetNewRegId();
        instructions.emplace_back(InsCode::loadi, m_val, m_reg);
        // set the register
        st.SetReg(m_val, m_reg);
    }
    return instructions;
}

std::vector<Instruction> VarAST::GenerateCode(SymbolTable &st) {
    std::vector<Instruction> instructions;
    m_reg = st.GetReg(m_name);
    // always load global symbols
    if (!st.CheckSymbol<true>(m_name) || m_reg == -1) {
        m_reg = st.GetNewRegId();
        instructions.emplace_back(InsCode::load, m_name, m_reg);
        st.SetReg(m_name, m_reg);
    }
    return instructions;
}

OpAST::OpAST(Token op, std::string sop) : m_op(op), m_sop(sop) {
    switch (m_op) {
        case Add:
            m_code = InsCode::add;
            m_sop = "add";
            m_precedence = 20;
            break;
        case Sub:
            m_code = InsCode::sub;
            m_sop = "sub";
            m_precedence = 20;
            break;
        case Mul:
            m_code = InsCode::mul;
            m_sop = "mul";
            m_precedence = 40;
            break;
        case Div:
            m_code = InsCode::div;
            m_sop = "div";
            m_precedence = 40;
            break;
        case Less:
            m_code = InsCode::lt;
            m_sop = "lt";
            m_precedence = 0;
            break;
        case More:
            m_code = InsCode::gt;
            m_sop = "gt";
            m_precedence = 0;
            break;
        case LessEq:
            m_code = InsCode::lteq;
            m_sop = "lteq";
            m_precedence = 0;
            break;
        case MoreEq:
            m_code = InsCode::gteq;
            m_sop = "gteq";
            m_precedence = 0;
            break;
        default:
            m_precedence = 0;
            break;
    }
}

std::vector<Instruction> OpAST::GenerateCode(SymbolTable &st) {
    auto left_tree = lhs->GenerateCode(st);
    auto right_tree = rhs->GenerateCode(st);

    std::vector<Instruction> operations;
    operations.reserve(left_tree.size() + right_tree.size() + 1);

    std::copy(left_tree.begin(), left_tree.end(),
              std::back_inserter(operations));
    std::copy(right_tree.begin(), right_tree.end(),
              std::back_inserter(operations));

    // binary operator will always define a new dest
    if (!m_dest) {
        m_reg = st.GetNewRegId();
    }
    operations.emplace_back(m_code, static_cast<int>(lhs->GetReg()),
                            static_cast<int>(rhs->GetReg()), m_reg);

    return operations;
}

}  // namespace sci
