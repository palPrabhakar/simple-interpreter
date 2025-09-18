#include <cstring>
#include <fstream>
#include <iostream>
#include <istream>
#include <stdexcept>
#include <string>

#include "ast.h"
#include "interpreter.h"
#include "optimizers/register_alloc.hpp"
#include "parser.h"
#include "symbol_table.h"
#include "tokenizer.h"

void PrintHelp() {
    std::cerr
        << "Usage: ./run --debug || ./run --ir || ./run --file <filename>\n";
    exit(0);
}

void mainLoop(std::istream &cmd_stream, bool debug, bool ir, bool file) {
    auto st = sci::SymbolTable();
    sci::Interpreter interpreter(st);

    std::string cmd;
    if (!file) {
        std::cout << ">>> ";
    }
    while (std::getline(cmd_stream, cmd)) {
        if (cmd == "exit") exit(0);

        sci::Tokenizer tokenizer(cmd);
        try {
            auto ast = sci::Parse(tokenizer, st);
            if (ast) {
                auto operations = ast->GenerateCode(st);
                operations = sci::do_register_alloc(std::move(operations));
                if (!ir) {
                    interpreter.Interpret(std::move(operations));

                    if (debug) {
                        for (auto &[k, v] : st.GetGlobalSymbols()) {
                            std::cout << std::format("{}: {}", k, v)
                                      << std::endl;
                        }
                    }

                } else {
                    auto ptr = dynamic_cast<sci::DummyAST *>(ast.get());
                    if (ptr) {
                        auto &fn_proto = st.GetPrototype(ptr->GetName());
                        operations = fn_proto->GetCode();
                    }

                    auto count = 0;
                    for (auto &op : operations) {
                        std::cout << count++ << ": " << op.ToString()
                                  << std::endl;
                    }
                }
            }
        } catch (std::runtime_error &err) {
            std::cout << err.what() << "\n";
        }

        if (!file) {
            std::cout << ">>> ";
        }
    }
}

int main(int argc, char **argv) {
    bool debug = false;
    bool ir = false;
    std::string file_name;

    if (argc != 1) {
        if (argc == 2 && std::strncmp("--debug", argv[1], 7) == 0) {
            debug = true;
        } else if (argc == 2 && std::strncmp("--ir", argv[1], 4) == 0) {
            ir = true;
        } else if (argc == 3 && std::strncmp("--file", argv[1], 6) == 0) {
            file_name = argv[2];
        } else {
            PrintHelp();
        }
    }

    if (!file_name.empty()) {
        std::ifstream file(file_name);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << file_name << "\n";
        }
        mainLoop(file, false, false, true);
    } else {
        mainLoop(std::cin, debug, ir, false);
    }

    return 0;
}
