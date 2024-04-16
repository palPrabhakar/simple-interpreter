#pragma once

#include "tokens.h"
#include <string>

namespace tci {
class Tokenizer {
public:
  Tokenizer(std::string file) : m_file(std::move(file)) {}

  std::pair<Token, std::string> GetNextToken();

  bool EOP() {
    return pos >= m_file.size();
  }

  void ResetTokenizer(std::string file) {
    m_file = std::move(file);
    pos = 0;
  }

  size_t GetPos() {
    return pos;
  }

private:
  std::string m_file;
  size_t pos = 0;
};
} // namespace tci
