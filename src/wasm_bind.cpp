#include <emscripten/bind.h>
#include "lexer.hpp"
#include "parser.hpp"
#include "compiler.hpp"
#include "vm.hpp"
#include <string>
#include <vector>

using namespace emscripten;

// Run the script and return all printed outputs as a JS array
std::vector<int> executeScript(const std::string& source) {
    std::vector<int> outputs;
    
    Lexer lexer(source);
    auto tokens = lexer.scanTokens();
    
    Parser parser(tokens);
    auto ast = parser.parse();
    
    Compiler compiler;
    auto bytecode = compiler.compile(ast);
    
    VM vm;
    // Remap the I/O callbacks!
    vm.onPrint = [&](int val) {
        outputs.push_back(val);
    };
    vm.onInput = []() {
        return 42; // Placeholder: In a real app, you'd pull from a JS queue
    };
    
    vm.run(bytecode, compiler.constants);
    return outputs;
}

EMSCRIPTEN_BINDINGS(cvm_module) {
    function("executeScript", &executeScript);
    register_vector<int>("VectorInt");
}