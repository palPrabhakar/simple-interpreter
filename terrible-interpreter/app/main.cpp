#include "tokenizer.h"
#include <iostream>
#include <string>

int main() {
  while (true) {
    std::string cmd;
    std::cout << ">>> ";
    std::getline(std::cin, cmd);

    if(cmd == "exit")
      exit(0);

    tci::Tokenizer tokenizer(cmd);

    while (!tokenizer.EOP()) {
      std::cout << tokenizer.GetNextToken().second << std::endl;
    }

    std::cout << cmd << "\n";
  }
  return 0;
}
