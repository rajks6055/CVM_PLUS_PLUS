#pragma once
#include "ast.hpp"
#include <vector>
#include <unordered_map>

class Compiler {
public:
    std::vector<uint8_t> compile(const std::vector<std::unique_ptr<Stmt>>& stmts);
    std::vector<int> constants;
private:
    std::vector<uint8_t> bytecode;
    std::unordered_map<std::string, uint8_t> globals; // Map var name to index
    uint8_t globalCount = 0;

    void emitByte(uint8_t byte);
    void emitBytes(uint8_t byte1, uint8_t byte2);
    size_t emitJump(uint8_t instruction);
    void patchJump(size_t offset);
    void emitLoop(size_t loopStart);

    void compileStmt(const Stmt& stmt);
    void compileExpr(const Expr& expr);
};