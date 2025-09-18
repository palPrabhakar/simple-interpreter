#pragma once

#include <cassert>
#include <deque>
#include <memory>
#include <string>
#include <unordered_map>

#include "function.h"

namespace sci {
using Symbols = std::unordered_map<std::string, double>;
using Registers = std::unordered_map<std::string, int>;
using Functions =
    std::unordered_map<std::string, std::unique_ptr<FunctionPrototype>>;

// TODO
// Make it Singleton
class SymbolTable {
   public:
    SymbolTable() { PushSymbolTable(); }
    SymbolTable(const SymbolTable& other) = delete;
    void operator=(const SymbolTable&) = delete;

    template <bool top_level = false>
    bool CheckSymbol(const std::string name) const {
        return m_st.back().contains(name) ||
               (!top_level && m_st.front().contains(name));
    }

    void Reset() {
        m_st.clear();
        m_rt.clear();
        m_rcount.clear();
        m_functions.clear();
        PushSymbolTable();
    }

    double GetValue(const std::string) const;

    void SetValue(const std::string, const double);
    void InsertSymbol(const std::string);

    int GetReg(const std::string) const;
    int GetReg(const double) const;

    void SetReg(const std::string, const int);
    void SetReg(const double, const int);

    void PushSymbolTable() {
        m_st.push_back(Symbols());
        m_rt.push_back(Registers());
        m_rcount.push_back(0);
    }

    void PopSymbolTable() {
        m_st.pop_back();
        m_rt.pop_back();
        m_rcount.pop_back();
    }

    const Symbols& GetTopLevelSymbols() const { return m_st.back(); }

    const Symbols& GetGlobalSymbols() const { return m_st.front(); }

    bool CheckFunction(const std::string name) const {
        return m_functions.contains(name);
    }

    uint GetNewRegId() { return (m_rcount.back())++; }

    uint GetRegIdx() { return m_rcount.back() - 1; }

    void InsertPrototype(const std::string name,
                         std::unique_ptr<FunctionPrototype> proto);

    const std::unique_ptr<FunctionPrototype>& GetPrototype(
        const std::string name) {
        return m_functions[name];
    }

   private:
    std::deque<Symbols> m_st;
    std::deque<Registers> m_rt;
    std::deque<uint> m_rcount;
    Functions m_functions;
};
}  // namespace sci
