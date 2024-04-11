#pragma once

namespace tci {
enum Token {
  Text,
  // Keywords
  Let,
  Mut,
  If,
  Else,
  For,
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
  End
};

} // namespace tci
