# 🚀 CVM++ 

![C++](https://img.shields.io/badge/Language-C++-blue.svg)
![Build](https://img.shields.io/badge/Build-Passing-brightgreen.svg)
![Architecture](https://img.shields.io/badge/Architecture-Stack--Based_VM-orange.svg)

CVM++ is an advanced, high-performance, multi-pass virtual machine and custom scripting language compiler written from scratch in C++. 

It follows strict phase-separation design patterns, featuring a custom lexical analyzer, a recursive descent predictive parser, an AST-to-Bytecode compiler, and a highly optimized stack-based execution engine.

---

## 🧠 Architectural Pipeline

The pipeline breaks down into four completely decoupled stages, maximizing scannability and low-level optimization:

1. **Lexical Analysis:** Converts raw source code strings into a validated Token stream with double-character lookahead (e.g., `<<`, `==`).
2. **Recursive Descent Parsing:** Consumes tokens and generates a memory-efficient Abstract Syntax Tree (AST).
3. **Bytecode Compilation:** Flattens the AST and handles structural backpatching to generate a raw `std::vector<uint8_t>` instruction array.
4. **Virtual Machine:** A fast, stack-based execution loop that dispatches operational codes (OpCodes) and handles memory state.

---

## ✨ Features

* **Turing Complete Control Flow:** Supports dynamic `while` loops and branching `if/else` conditionals.
* **Extended Instruction Set (Bitwise):** Full support for integer mathematics and bitwise capability operators: `&` (AND), `|` (OR), `^` (XOR), `<<` (Left Shift), and `>>` (Right Shift).
* **Decoupled I/O:** The execution engine is completely abstracted away from standard platform streams, allowing the VM to be easily embedded in other applications or exported to WebAssembly (WASM).
* **Dynamic Variable State:** Lexical assignment and global memory registry using the `let` keyword.

---

## 📂 Project Structure

    CVM_PROJECT/
    ├── src/
    │   ├── common.hpp         # Central OpCodes, Tokens, and Data contracts
    │   ├── lexer.hpp/.cpp     # Tokenizing engine 
    │   ├── ast.hpp            # Value-type focused AST structures
    │   ├── parser.hpp/.cpp    # Recursive descent predictive parser
    │   ├── compiler.hpp/.cpp  # AST flattener and structural backpatching
    │   ├── vm.hpp/.cpp        # Low-level execution loop and memory stack
    │   └── main.cpp           # Native CLI compiler/runner entrypoint
    ├── tests/
    │   └── test_all.cvm       # Comprehensive feature validation script
    └── run_tests.ps1          # Automated Windows testing suite script

---

## 🛠️ Building from Source

### Prerequisites
A standard C++ compiler (GCC, Clang, or MSVC). No external dependencies or libraries are required.

### Compilation
To build the CLI tool natively with aggressive optimization flags (`-O3`), run the following from the root directory:

    g++ -O3 src/lexer.cpp src/parser.cpp src/compiler.cpp src/vm.cpp src/main.cpp -o cvm++


---

## 💻 Language Syntax Guide

The custom CVM language supports dynamic local state variables, console streams, conditionals, and mathematical evaluations. Here is a valid code sample illustrating its scope:

    # Variable registration and user input stream
    print 8888; 
    let input_val = input;

    # Extended bitwise logic
    let shifted = input_val << 2;
    let masked = shifted & 15;

    # Branching logic structures
    if masked < 10 {
        print 99;
    } else {
        print 0;
    }

    # Iterative execution blocks
    let i = 0;
    while i < 3 {
        print i;
        i = i + 1;
    }

---

## 🧪 Testing

The codebase features an automated testing script for environment validation on Windows.
Simply run:

    .\run_tests.ps1

This runs the engine against `tests/test_all.cvm`, executes the bytecode, and outputs the calculation results to a local `.txt` artifact folder.
