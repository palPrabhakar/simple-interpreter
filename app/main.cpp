#include <iostream>
#include <stdexcept>
#include <string>

#include "ast.h"
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
      if (ast) {
        uint ridx = 1;

        auto operations = ast->GenerateCode(ridx);
        if (!debug) {
          interpreter.Interpret(std::move(operations));

          for (auto &[k, v] : st.GetGlobalSymbols()) {
            std::cout << std::format("{}: {}", k, v) << std::endl;
          }

        } else {
          auto ptr = dynamic_cast<sci::DummyAST *>(ast.get());
          if (ptr) {
            auto& fn_proto = st.GetPrototype(ptr->GetName());
            operations = fn_proto->GetCode();
          }

          auto count = 0;
          for (auto &op : operations) {
            std::cout << count++ << ": " << op.ToString() << std::endl;
          }
        }
      }
    } catch (std::runtime_error &err) {
      std::cout << err.what() << "\n";
    }
  }
  return 0;
}
