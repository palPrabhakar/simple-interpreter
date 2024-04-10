#pragma once

namespace tci {
enum Token {
  Text,
  // Keywords
  Let,
  If,
  Else,
  For,
  While,
  Do,
  Continue,
  Break,
  // Operators
  Add,
  Sub,
  Mul,
  Div,
  Mod,
  Assign,
  Equal,
  NEqual,
  Less,
  More,
  LessEq,
  MoreEq,
  And,
  Or,
  SAnd,
  SOr,
  Inc,
  Dec,
  // Others
  LBrack,
  LParen,
  LSBrack,
  RBrack,
  RParen,
  RSBrack,
  SColon,
  Fn,
  End
};

} // namespace tci
