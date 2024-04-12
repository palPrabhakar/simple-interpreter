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
    auto ast = tci::Parse(tokenizer, st);
    uint ridx = 0;
    auto operations = ast->GenerateCodeStr(ridx);
    auto count = 0;
    for (auto op : operations) {
      if (op[0] == '#') {
        continue;
      }

      std::cout << count++ << ": " << op << std::endl;
    }
  }
  return 0;
}
