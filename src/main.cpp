#include "lexer.hpp"
#include "parser.hpp"
#include "compiler.hpp"
#include "vm.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: cvm++ [script.cvm]\n";
        return 1;
    }

    // Explicitly verify if the file exists and can be opened
    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "CRITICAL ERROR: Could not open or find file '" << argv[1] << "'\n";
        std::cerr << "Please verify the file exists and your path is correct.\n";
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    if (source.empty()) {
        std::cout << "Notification: Source file is empty. Nothing to execute.\n";
        return 0;
    }

    try {
        Lexer lexer(source);
        auto tokens = lexer.scanTokens();

        Parser parser(tokens);
        auto ast = parser.parse();

        Compiler compiler;
        auto bytecode = compiler.compile(ast);

        VM vm;
        vm.run(bytecode, compiler.constants);
    } 
    catch (const std::exception& e) {
        std::cerr << "ENGINE ERROR: " << e.what() << "\n";
        return 1;
    }

    return 0;
}