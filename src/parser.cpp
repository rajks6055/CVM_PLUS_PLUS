#include "parser.hpp"
#include <stdexcept>

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

std::vector<std::unique_ptr<Stmt>> Parser::parse() {
    std::vector<std::unique_ptr<Stmt>> statements;
    while (!check(TokenType::EOF_TOKEN)) {
        statements.push_back(declaration());
    }
    return statements;
}

std::unique_ptr<Stmt> Parser::declaration() {
    if (match({TokenType::LET})) {
        consume(TokenType::IDENTIFIER, "Expect variable name.");
        std::string name = previous().lexeme;
        std::unique_ptr<Expr> init = nullptr;
        if (match({TokenType::EQ})) { init = expression(); }
        consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
        auto stmt = std::make_unique<Stmt>();
        stmt->type = StmtType::VAR_DECL;
        stmt->varName = name;
        stmt->initializer = std::move(init);
        return stmt;
    }
    return statement();
}

std::unique_ptr<Stmt> Parser::statement() {
    if (match({TokenType::IF})) return ifStatement();
    if (match({TokenType::WHILE})) return whileStatement();
    if (match({TokenType::PRINT})) {
        auto expr = expression();
        consume(TokenType::SEMICOLON, "Expect ';' after value.");
        auto stmt = std::make_unique<Stmt>();
        stmt->type = StmtType::PRINT;
        stmt->expr = std::move(expr);
        return stmt;
    }
    if (match({TokenType::LBRACE})) return block();
    
    if (check(TokenType::IDENTIFIER) && current + 1 < tokens.size() && tokens[current + 1].type == TokenType::EQ) {
        Token name = advance();
        advance(); 
        auto val = expression();
        consume(TokenType::SEMICOLON, "Expect ';' after assignment.");
        auto stmt = std::make_unique<Stmt>();
        stmt->type = StmtType::ASSIGN;
        stmt->varName = name.lexeme;
        stmt->value = std::move(val);
        return stmt;
    }

    auto expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';'.");
    auto stmt = std::make_unique<Stmt>();
    stmt->type = StmtType::PRINT; 
    stmt->expr = std::move(expr);
    return stmt;
}

std::unique_ptr<Stmt> Parser::ifStatement() {
    auto condition = expression();
    auto thenBranch = statement();
    std::unique_ptr<Stmt> elseBranch = nullptr;
    if (match({TokenType::ELSE})) {
        elseBranch = statement();
    }
    auto stmt = std::make_unique<Stmt>();
    stmt->type = StmtType::IF;
    stmt->condition = std::move(condition);
    stmt->thenBranch = std::move(thenBranch);
    stmt->elseBranch = std::move(elseBranch);
    return stmt;
}

std::unique_ptr<Stmt> Parser::whileStatement() {
    auto condition = expression();
    auto body = statement();
    auto stmt = std::make_unique<Stmt>();
    stmt->type = StmtType::WHILE;
    stmt->condition = std::move(condition);
    stmt->body = std::move(body);
    return stmt;
}

std::unique_ptr<Stmt> Parser::block() {
    std::vector<std::unique_ptr<Stmt>> statements;
    while (!check(TokenType::RBRACE) && !check(TokenType::EOF_TOKEN)) {
        statements.push_back(declaration());
    }
    consume(TokenType::RBRACE, "Expect '}' after block.");
    auto stmt = std::make_unique<Stmt>();
    stmt->type = StmtType::BLOCK;
    stmt->statements = std::move(statements);
    return stmt;
}

std::unique_ptr<Expr> Parser::expression() { return equality(); }

std::unique_ptr<Expr> Parser::equality() {
    auto expr = bitwise();
    while (match({TokenType::EQ_EQ, TokenType::LESS})) {
        Token op = previous();
        auto right = bitwise();
        auto newExpr = std::make_unique<Expr>();
        newExpr->type = ExprType::BINARY;
        newExpr->left = std::move(expr);
        newExpr->op = op;
        newExpr->right = std::move(right);
        expr = std::move(newExpr);
    }
    return expr;
}

std::unique_ptr<Expr> Parser::bitwise() {
    auto expr = term();
    while (match({TokenType::AMPERSAND, TokenType::PIPE, TokenType::CARET, TokenType::SHL, TokenType::SHR})) {
        Token op = previous();
        auto right = term();
        auto newExpr = std::make_unique<Expr>();
        newExpr->type = ExprType::BINARY;
        newExpr->left = std::move(expr);
        newExpr->op = op;
        newExpr->right = std::move(right);
        expr = std::move(newExpr);
    }
    return expr;
}

std::unique_ptr<Expr> Parser::term() {
    auto expr = factor();
    while (match({TokenType::PLUS, TokenType::MINUS})) {
        Token op = previous();
        auto right = factor();
        auto newExpr = std::make_unique<Expr>();
        newExpr->type = ExprType::BINARY;
        newExpr->left = std::move(expr);
        newExpr->op = op;
        newExpr->right = std::move(right);
        expr = std::move(newExpr);
    }
    return expr;
}

std::unique_ptr<Expr> Parser::factor() {
    auto expr = primary();
    while (match({TokenType::STAR, TokenType::SLASH})) {
        Token op = previous();
        auto right = primary();
        auto newExpr = std::make_unique<Expr>();
        newExpr->type = ExprType::BINARY;
        newExpr->left = std::move(expr);
        newExpr->op = op;
        newExpr->right = std::move(right);
        expr = std::move(newExpr);
    }
    return expr;
}

std::unique_ptr<Expr> Parser::primary() {
    auto expr = std::make_unique<Expr>();
    if (match({TokenType::NUMBER})) {
        expr->type = ExprType::LITERAL;
        expr->literalValue = std::stoi(previous().lexeme);
        return expr;
    }
    if (match({TokenType::IDENTIFIER})) {
        expr->type = ExprType::VARIABLE;
        expr->varName = previous().lexeme;
        return expr;
    }
    if (match({TokenType::INPUT})) {
        expr->type = ExprType::INPUT;
        return expr;
    }
    throw std::runtime_error("Expect expression.");
}

bool Parser::match(std::initializer_list<TokenType> types) {
    for (TokenType type : types) {
        if (check(type)) { advance(); return true; }
    }
    return false;
}
bool Parser::check(TokenType type) { return check(TokenType::EOF_TOKEN) ? false : peek().type == type; }
Token Parser::advance() { if (!check(TokenType::EOF_TOKEN)) current++; return previous(); }
Token Parser::peek() { return tokens[current]; }
Token Parser::previous() { return tokens[current - 1]; }
void Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) advance();
    else throw std::runtime_error(message);
}