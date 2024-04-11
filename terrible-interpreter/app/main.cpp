#include <iostream>
#include <string>

#include "parser.h"
#include "symbol_table.h"
#include "tokenizer.h"

int main() {
  tci::SymbolTable st;
  while (true) {
    std::string cmd;
    std::cout << ">>> ";
    std::getline(std::cin, cmd);

    if (cmd == "exit") exit(0);

    tci::Tokenizer tokenizer(cmd);
    tci::Parse(tokenizer, st);
  }
  return 0;
}
