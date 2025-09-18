#include "symbol_table.h"

namespace sci {
double SymbolTable::GetValue(const std::string name) const {
    if (m_st.back().contains(name)) {
        return m_st.back().at(name);
    }
    return m_st.front().at(name);
}

void SymbolTable::SetValue(const std::string name, const double val) {
    if (m_st.back().contains(name)) {
        m_st.back()[name] = val;
    } else {
        m_st.front()[name] = val;
    }
}

int SymbolTable::GetReg(const std::string name) const {
    if (m_rt.back().contains(name)) {
        return m_rt.back().at(name);
    }
    return -1;
}

int SymbolTable::GetReg(const double literal) const {
    auto key = std::to_string(literal);
    if(m_rt.back().contains(key)) {
        return m_rt.back().at(key);
    }
    return -1;
}

void SymbolTable::SetReg(const std::string name, const int val) {
    m_rt.back()[name] = val;
}

void SymbolTable::SetReg(const double literal, const int val) {
    m_rt.back()[std::to_string(literal)] = val;
}


void SymbolTable::InsertSymbol(const std::string name) {
    m_st.back().insert({name, 0});
}

void SymbolTable::InsertPrototype(std::string name,
                                  std::unique_ptr<FunctionPrototype> func) {
    m_functions.insert({name, std::move(func)});
}

}  // namespace sci
