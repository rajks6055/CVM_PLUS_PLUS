# 🚀 CVM++

![C++](https://img.shields.io/badge/Language-C++17-blue.svg)
![Build](https://img.shields.io/badge/Build-Passing-brightgreen.svg)
![Architecture](https://img.shields.io/badge/Architecture-Stack--Based_VM-orange.svg)
![License](https://img.shields.io/badge/License-Educational-lightgrey.svg)

**CVM++** is a high-performance, lightweight custom scripting language and stack-based Virtual Machine (VM) written entirely from scratch in modern C++. 

Designed with strict phase-separation patterns, this project serves as both a fast execution engine and a perfect educational resource for understanding compiler design, bytecode generation, and virtual machine architecture.

---

## ✨ Key Features

* **Complete Compilation Pipeline:** A decoupled architecture featuring a custom Lexer, a recursive descent Parser, an AST-to-Bytecode Compiler, and the VM.
* **Turing Complete:** Full support for dynamic `while` loops, branching `if/else` conditionals, and local variable state management.
* **Extended Instruction Set:** Robust support for arithmetic math and bitwise operations (`&`, `|`, `^`, `<<`, `>>`).
* **Developer Tooling:** Built-in debugging modes to visualize the Abstract Syntax Tree (AST) and trace bytecode execution in real-time.
* **Decoupled I/O:** The execution loop is abstracted from standard platform streams, making it highly portable and embeddable.
* **Safe & Robust:** Comprehensive error reporting across all compilation stages (e.g., syntax errors, undefined variables, stack underflows).

---

## ⚙️ System Requirements

To compile and run CVM++, you need a lightweight development environment:
* **C++ Standard:** C++17 or higher.
* **Compiler:** Any modern compiler (GCC 7+, Clang 5+, or MSVC 2017+).
* **Dependencies:** None! The project relies entirely on the C++ Standard Library (`<iostream>`, `<vector>`, `<fstream>`, etc.).

---

## 🧠 Architecture Overview

The CVM++ pipeline is broken down into four distinct, memory-efficient stages. This maximizes scannability and allows for low-level optimizations at each step.

    📄 Source Code (.cvm)
            │
            ▼
    🔍 1. Lexical Analyzer   ➔ Validates strings and generates a Token stream with lookahead.
            │
            ▼
    🌳 2. Parser             ➔ Consumes tokens to build a memory-efficient Abstract Syntax Tree.
            │
            ▼
    ⚙️ 3. Compiler           ➔ Flattens the AST & handles backpatching to emit raw bytecode.
            │
            ▼
    🚀 4. Virtual Machine    ➔ Stack-based execution loop that dispatches OpCodes & memory.

---

## 📂 Project Structure

    CVM_PROJECT/
    ├── src/
    │   ├── common.hpp         # Central OpCodes, Tokens, and Data contracts
    │   ├── lexer.hpp/.cpp     # Tokenizing engine for lexical analysis
    │   ├── ast.hpp            # Value-type focused AST node structures
    │   ├── parser.hpp/.cpp    # Recursive descent predictive parser
    │   ├── compiler.hpp/.cpp  # AST flattener and bytecode generator
    │   ├── vm.hpp/.cpp        # Low-level execution loop and memory stack
    │   └── main.cpp           # Native CLI compiler/runner entrypoint
    ├── scripts/               # Example .cvm scripts for testing
    ├── tests/
    │   └── test_all.cvm       # Comprehensive feature validation script
    └── run_tests.ps1          # Automated Windows testing suite script

---

## 🛠️ Building & Installation

Because CVM++ has zero external dependencies, compilation is incredibly straightforward. Navigate to the project root and compile natively with aggressive optimization flags:

    # Compile using GCC
    g++ -std=c++17 -O3 src/lexer.cpp src/parser.cpp src/compiler.cpp src/vm.cpp src/main.cpp -o cvm

---

## 💻 Usage & CLI Tools

Once compiled, you can execute `.cvm` scripts directly through the terminal. The CLI also includes powerful debugging flags for developers.

**Basic Execution:**

    ./cvm script.cvm

**Developer Debugging Flags:**
* `./cvm script.cvm --ast` ➔ Dumps the generated Abstract Syntax Tree to the console.
* `./cvm script.cvm --debug` ➔ Traces bytecode instructions step-by-step during execution.
* `./cvm script.cvm --ast --debug` ➔ Outputs both the AST and the execution trace.

---

## 📖 Language Syntax Guide

The CVM++ scripting language is designed to be clean and intuitive. Here is an example showcasing variable registration, standard I/O, bitwise logic, and control flow:

    # hello.cvm

    print 8888; 
    let input_val = input;      # Read from standard input

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
    let count = 3;
    while count > 0 {
        print count;
        count = count - 1;
    }
    print 0;

---

## 🧮 Virtual Machine Specification

The core of the VM is powered by a highly optimized, stack-based OpCode dispatcher. 

| OpCode | Arguments | Description |
| :--- | :--- | :--- |
| `PUSH` | `int32` | Push a literal 32-bit integer onto the stack |
| `POP` | — | Discard the top value of the stack |
| `LOAD` | `var_idx` | Push a stored variable's value onto the stack |
| `STORE` | `var_idx` | Pop the top of the stack and store it in a variable |
| `ADD` / `SUB` | — | Pop two values, perform arithmetic, push result |
| `MUL` / `DIV` | — | Pop two values, perform arithmetic, push result |
| `EQ` / `LT` | — | Pop two values, perform comparison, push boolean (1/0) |
| `JUMP` | `addr` | Unconditional forward branch to an address |
| `JUMP_IF_FALSE`| `addr` | Conditional branch (pops top of stack, jumps if 0) |
| `PRINT` | — | Pop and output the top of the stack to standard out |
| `INPUT` | — | Read an integer from standard in, push to stack |
| `HALT` | — | Terminate VM execution |

---

## 🤝 Contributing

Contributions are highly welcome! Whether you are fixing bugs or adding new language features, here is how you can help:

1. **Report Bugs:** Open an issue if you encounter stack underflows or parsing errors.
2. **Suggest Features:** Propose optimizations or new syntax elements.
3. **Submit Code:** Fork the repository, ensure your code adheres to C++17 standards, add inline comments for complex logic, and submit a Pull Request.

*Note: Please ensure all changes pass the automated test suite (`.\run_tests.ps1`) before submitting.*

---

## 📄 License

This project is provided as-is for educational, development, and learning purposes. Feel free to fork, modify, and experiment!
