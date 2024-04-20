#include <iostream>
#include <stdexcept>
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

  auto st = sci::SymbolTable();
  sci::Interpreter interpreter(st);
  while (true) {
    std::string cmd;
    std::cout << ">>> ";
    std::getline(std::cin, cmd);

    if (cmd == "exit") exit(0);

    sci::Tokenizer tokenizer(cmd);
    try {
      auto ast = sci::Parse(tokenizer, st);
      // idx 0 fixed for rax
      uint ridx = 1;

      auto operations = ast->GenerateCode(ridx);
      if (!debug) {
        interpreter.Interpret(std::move(operations));

        for (auto &[k, v] : st.GetGlobalSymbols()) {
          std::cout << std::format("{}: {}", k, v) << std::endl;
        }

      } else {
        // auto operations = ast->GenerateCodeStr(ridx);
        auto count = 0;
        for (auto &op : operations) {
          std::cout << count++ << ": " << op.ToString() << std::endl;
        }
      }
    } catch (std::runtime_error &err) {
      std::cout << err.what() << "\n";
    }
  }
  return 0;
}
