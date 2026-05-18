#pragma once
#include "common.hpp"
#include <vector>
#include <string>

class Lexer {
public:
    Lexer(const std::string& source);
    std::vector<Token> scanTokens();
private:
    std::string source;
    size_t start = 0;
    size_t current = 0;
    int line = 1;

    void scanToken(std::vector<Token>& tokens);
    char advance();
    bool match(char expected);
    char peek();
    void addToken(std::vector<Token>& tokens, TokenType type);
};