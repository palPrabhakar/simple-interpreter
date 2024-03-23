#include "tokenizer.h"
#include <cctype>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace tci {
const std::unordered_map<std::string, Token> token_map = {
    {"int", Token::Int},     {"double", Token::Double},
    {"char", Token::Char},   {"void", Token::Void},
    {"if", Token::If},       {"else", Token::Else},
    {"for", Token::For},     {"while", Token::While},
    {"do", Token::Do},       {"continue", Token::Continue},
    {"break", Token::Break}, {"+", Token::Add},
    {"-", Token::Sub},       {"*", Token::Mul},
    {"/", Token::Div},       {"%", Token::Mod},
    {"=", Token::Assign},    {"==", Token::Equal},
    {"!=", Token::NEqual},   {"<", Token::Less},
    {">", Token::More},      {"<=", Token::LessEq},
    {">=", Token::MoreEq},   {"&&", Token::And},
    {"||", Token::Or},       {"&", Token::SAnd},
    {"|", Token::SOr},       {"++", Token::Inc},
    {"--", Token::Dec},      {"{", Token::LParen},
    {"}", Token::RParen},    {"(", Token::LBrack},
    {")", Token::RBrack},    {"[", Token::LSBrack},
    {"]", Token::RSBrack},   {";", Token::SColon}};

const std::unordered_set<char> stop_words = {';', '(', ')', '{', '}', ' ', '[',
                                             ']', '+', '-', '*', '/', '%', '=',
                                             '!', '<', '>', '&', '|'};

const std::unordered_set<char> sp_words = {'=', '!', '>', '<', '+', '-'};

std::pair<Token, std::string> Tokenizer::GetNextToken() {
  std::string word;
  Token token;

  // get rid of any whitespace
  while (pos < m_file.size() && std::isspace(m_file[pos])) {
    pos++;
  }

  // return end token
  if (pos == m_file.size()) {
    return {End, word};
  }

  if (pos + 1 < m_file.size() && sp_words.contains(m_file[pos])) {
    word = m_file[pos];
    if (m_file[pos] == '!' && m_file[pos + 1] == '=') {
      word += m_file[++pos];
    } else if (m_file[pos] == m_file[pos + 1]) {
      word += m_file[++pos];
    }
    ++pos;
    token = token_map.at(word);
    return {token, word};
  }

  if (stop_words.contains(m_file[pos])) {
    word = m_file[pos];
    ++pos;
    token = token_map.at(word);
    return {token, word};
  }

  for (; pos + 1 < m_file.size(); ++pos) {
    word += m_file[pos];
    if (stop_words.contains(m_file[pos + 1])) {
      ++pos;
      break;
    }
  }

  if (token_map.contains(word)) {
    token = token_map.at(word);
  } else {
    token = Text;
  }

  return {token, word};
}

} // namespace tci
