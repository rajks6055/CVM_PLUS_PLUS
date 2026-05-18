#include "compiler.hpp"

std::vector<uint8_t> Compiler::compile(const std::vector<std::unique_ptr<Stmt>>& stmts) {
    for (const auto& stmt : stmts) {
        compileStmt(*stmt);
    }
    emitByte(OP_HALT);
    return bytecode;
}

void Compiler::emitByte(uint8_t byte) { bytecode.push_back(byte); }
void Compiler::emitBytes(uint8_t byte1, uint8_t byte2) { emitByte(byte1); emitByte(byte2); }

size_t Compiler::emitJump(uint8_t instruction) {
    emitByte(instruction);
    emitByte(0xff);
    emitByte(0xff);
    return bytecode.size() - 2;
}

void Compiler::patchJump(size_t offset) {
    size_t jump = bytecode.size() - offset - 2;
    bytecode[offset] = (jump >> 8) & 0xff;
    bytecode[offset + 1] = jump & 0xff;
}

void Compiler::emitLoop(size_t loopStart) {
    emitByte(OP_LOOP);
    size_t offset = bytecode.size() - loopStart + 2;
    emitByte((offset >> 8) & 0xff);
    emitByte(offset & 0xff);
}

void Compiler::compileStmt(const Stmt& stmt) {
    switch (stmt.type) {
        case StmtType::PRINT:
            compileExpr(*stmt.expr);
            emitByte(OP_PRINT);
            break;
        case StmtType::VAR_DECL:
            if (stmt.initializer) compileExpr(*stmt.initializer);
            globals[stmt.varName] = globalCount;
            emitBytes(OP_SET_GLOBAL, globalCount++);
            break;
        case StmtType::ASSIGN:
            compileExpr(*stmt.value);
            emitBytes(OP_SET_GLOBAL, globals[stmt.varName]);
            break;
        case StmtType::BLOCK:
            for (const auto& s : stmt.statements) compileStmt(*s);
            break;
        case StmtType::IF: {
            compileExpr(*stmt.condition);
            size_t thenJump = emitJump(OP_JUMP_IF_FALSE);
            emitByte(OP_POP);
            compileStmt(*stmt.thenBranch);
            size_t elseJump = emitJump(OP_JUMP);
            patchJump(thenJump);
            emitByte(OP_POP);
            if (stmt.elseBranch) compileStmt(*stmt.elseBranch);
            patchJump(elseJump);
            break;
        }
        case StmtType::WHILE: {
            size_t loopStart = bytecode.size();
            compileExpr(*stmt.condition);
            size_t exitJump = emitJump(OP_JUMP_IF_FALSE);
            emitByte(OP_POP);
            compileStmt(*stmt.body);
            emitLoop(loopStart);
            patchJump(exitJump);
            emitByte(OP_POP);
            break;
        }
    }
}

void Compiler::compileExpr(const Expr& expr) {
    switch (expr.type) {
        case ExprType::LITERAL:
            constants.push_back(expr.literalValue);
            emitBytes(OP_CONSTANT, constants.size() - 1);
            break;
        case ExprType::VARIABLE:
            emitBytes(OP_GET_GLOBAL, globals[expr.varName]);
            break;
        case ExprType::INPUT:
            emitByte(OP_INPUT);
            break;
        case ExprType::BINARY:
            compileExpr(*expr.left);
            compileExpr(*expr.right);
            switch (expr.op.type) {
                case TokenType::PLUS: emitByte(OP_ADD); break;
                case TokenType::MINUS: emitByte(OP_SUB); break;
                case TokenType::STAR: emitByte(OP_MUL); break;
                case TokenType::SLASH: emitByte(OP_DIV); break;
                case TokenType::EQ_EQ: emitByte(OP_EQ); break;
                case TokenType::LESS: emitByte(OP_LESS); break;
                case TokenType::AMPERSAND: emitByte(OP_BIT_AND); break;
                case TokenType::PIPE: emitByte(OP_BIT_OR); break;
                case TokenType::CARET: emitByte(OP_BIT_XOR); break;
                case TokenType::SHL: emitByte(OP_SHL); break;
                case TokenType::SHR: emitByte(OP_SHR); break;
                default: break;
            }
            break;
    }
}