#pragma once
#include "common.hpp"
#include <string>
#include <vector>
#include <memory>

enum class ExprType { LITERAL, VARIABLE, BINARY, INPUT };

struct Expr {
    ExprType type;
    int literalValue = 0;
    std::string varName = "";
    std::unique_ptr<Expr> left = nullptr;
    Token op;
    std::unique_ptr<Expr> right = nullptr;
};

enum class StmtType { PRINT, VAR_DECL, ASSIGN, BLOCK, IF, WHILE };

struct Stmt {
    StmtType type;
    std::unique_ptr<Expr> expr = nullptr;
    std::string varName = "";
    std::unique_ptr<Expr> initializer = nullptr;
    std::unique_ptr<Expr> value = nullptr;
    std::vector<std::unique_ptr<Stmt>> statements;
    std::unique_ptr<Expr> condition = nullptr;
    std::unique_ptr<Stmt> thenBranch = nullptr;
    std::unique_ptr<Stmt> elseBranch = nullptr;
    std::unique_ptr<Stmt> body = nullptr;
};