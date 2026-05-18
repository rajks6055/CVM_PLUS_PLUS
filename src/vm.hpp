#pragma once
#include <vector>
#include <cstdint>
#include <functional>

class VM {
public:
    VM();
    
    // Decoupled I/O for WASM readiness
    std::function<void(int)> onPrint;
    std::function<int()> onInput;

    void run(const std::vector<uint8_t>& bytecode, const std::vector<int>& constants);

private:
    static const int STACK_MAX = 256;
    int stack[STACK_MAX];
    int* stackTop;
    int globals[256];

    inline void push(int value) { *stackTop++ = value; }
    inline int pop() { return *--stackTop; }
    inline int peek(int distance) { return stackTop[-1 - distance]; }
};