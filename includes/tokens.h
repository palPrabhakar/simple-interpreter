#pragma once

namespace sci {
enum Token {
  Text,
  // Keywords
  Let,
  Mut,
  If,
  Else,
  // For, --> no support for for-loop
  While,
  Continue,
  Break,
  Return,
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
  Comma,
  Fn,
  Call,
  End
};

} // namespace sci
