#include <iostream>
#include <string>

#include "interpreter.h"
#include "parser.h"
#include "symbol_table.h"
#include "tokenizer.h"

int main(int argc, char **argv) {
  bool debug = false;
  if (argc > 1) {
    debug = true;
  }

  auto st = tci::SymbolTable();
  tci::Interpreter interpreter(st);
  while (true) {
    std::string cmd;
    std::cout << ">>> ";
    std::getline(std::cin, cmd);

    if (cmd == "exit") exit(0);

    tci::Tokenizer tokenizer(cmd);
    auto ast = tci::Parse(tokenizer, st, debug);
    // idx 0 fixed for rax
    uint ridx = 1;

    if (!debug) {
      auto operations = ast->GenerateCode(ridx);
      interpreter.Interpret(std::move(operations));

      for (auto &[k, v] : st.GetGlobalSymbols()) {
        std::cout << std::format("{}: {}", k, v) << std::endl;
      }

    } else {
      auto operations = ast->GenerateCodeStr(ridx);
      auto count = 0;
      for (auto op : operations) {
        if (op[0] == '#') {
          continue;
        }
        std::cout << count++ << ": " << op << std::endl;
      }
    }
  }
  return 0;
}