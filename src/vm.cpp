#include "vm.hpp"
#include "common.hpp"
#include <iostream>

VM::VM() {
    onPrint = [](int v) { std::cout << v << "\n"; };
    onInput = []() { int v; std::cin >> v; return v; };
}

void VM::run(const std::vector<uint8_t>& code, const std::vector<int>& constants) {
    stackTop = stack;
    const uint8_t* ip = code.data();

// Direct Threaded Code Optimizations
#ifdef __GNUC__
    #define DISPATCH() goto *dispatch_table[*ip++]
    #define BEGIN_DISPATCH \
        static void* dispatch_table[] = { \
            &&do_OP_CONSTANT, &&do_OP_GET_GLOBAL, &&do_OP_SET_GLOBAL, \
            &&do_OP_ADD, &&do_OP_SUB, &&do_OP_MUL, &&do_OP_DIV, \
            &&do_OP_EQ, &&do_OP_LESS, \
            &&do_OP_BIT_AND, &&do_OP_BIT_OR, &&do_OP_BIT_XOR, &&do_OP_SHL, &&do_OP_SHR, \
            &&do_OP_PRINT, &&do_OP_INPUT, \
            &&do_OP_JUMP, &&do_OP_JUMP_IF_FALSE, &&do_OP_LOOP, \
            &&do_OP_POP, &&do_OP_HALT \
        }; \
        DISPATCH();
    #define CASE(op) do_##op:
#else
    // Standard Switch fallback for MSVC
    #define DISPATCH() break
    #define BEGIN_DISPATCH for(;;) { switch(*ip++) {
    #define CASE(op) case op:
#endif

#define BINARY_OP(op) \
    do { \
        int b = pop(); int a = pop(); \
        push(a op b); \
    } while (false)

    BEGIN_DISPATCH
        CASE(OP_CONSTANT) { push(constants[*ip++]); DISPATCH(); }
        CASE(OP_GET_GLOBAL) { push(globals[*ip++]); DISPATCH(); }
        CASE(OP_SET_GLOBAL) { globals[*ip++] = peek(0); DISPATCH(); }
        
        CASE(OP_ADD) { BINARY_OP(+); DISPATCH(); }
        CASE(OP_SUB) { BINARY_OP(-); DISPATCH(); }
        CASE(OP_MUL) { BINARY_OP(*); DISPATCH(); }
        CASE(OP_DIV) { BINARY_OP(/); DISPATCH(); }
        
        CASE(OP_EQ) { BINARY_OP(==); DISPATCH(); }
        CASE(OP_LESS) { BINARY_OP(<); DISPATCH(); }
        
        // Bitwise Extended Set
        CASE(OP_BIT_AND) { BINARY_OP(&); DISPATCH(); }
        CASE(OP_BIT_OR)  { BINARY_OP(|); DISPATCH(); }
        CASE(OP_BIT_XOR) { BINARY_OP(^); DISPATCH(); }
        CASE(OP_SHL)     { BINARY_OP(<<); DISPATCH(); }
        CASE(OP_SHR)     { BINARY_OP(>>); DISPATCH(); }

        CASE(OP_PRINT) { onPrint(pop()); DISPATCH(); }
        CASE(OP_INPUT) { push(onInput()); DISPATCH(); }
        
        CASE(OP_JUMP) {
            uint16_t offset = (ip[0] << 8) | ip[1];
            ip += 2 + offset;
            DISPATCH();
        }
        CASE(OP_JUMP_IF_FALSE) {
            uint16_t offset = (ip[0] << 8) | ip[1];
            ip += 2;
            if (peek(0) == 0) ip += offset;
            DISPATCH();
        }
        CASE(OP_LOOP) {
            uint16_t offset = (ip[0] << 8) | ip[1];
            ip += 2;
            ip -= offset;
            DISPATCH();
        }
        CASE(OP_POP) { pop(); DISPATCH(); }
        CASE(OP_HALT) { return; }

#ifndef __GNUC__
    } } // End switch loop
#endif
}