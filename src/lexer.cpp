#include "lexer.hpp"
#include <cctype>

Lexer::Lexer(const std::string& source) : source(source) {}

std::vector<Token> Lexer::scanTokens() {
    std::vector<Token> tokens;
    while (current < source.length()) {
        start = current;
        scanToken(tokens);
    }
    tokens.push_back({TokenType::EOF_TOKEN, "", line});
    return tokens;
}

char Lexer::advance() { return source[current++]; }
bool Lexer::match(char expected) {
    if (current >= source.length() || source[current] != expected) return false;
    current++; return true;
}
char Lexer::peek() { return current >= source.length() ? '\0' : source[current]; }

void Lexer::addToken(std::vector<Token>& tokens, TokenType type) {
    tokens.push_back({type, source.substr(start, current - start), line});
}

void Lexer::scanToken(std::vector<Token>& tokens) {
    char c = advance();
    switch (c) {
        case '(': addToken(tokens, TokenType::LPAREN); break;
        case ')': addToken(tokens, TokenType::RPAREN); break;
        case '{': addToken(tokens, TokenType::LBRACE); break;
        case '}': addToken(tokens, TokenType::RBRACE); break;
        case ';': addToken(tokens, TokenType::SEMICOLON); break;
        case '+': addToken(tokens, TokenType::PLUS); break;
        case '-': addToken(tokens, TokenType::MINUS); break;
        case '*': addToken(tokens, TokenType::STAR); break;
        case '/': addToken(tokens, TokenType::SLASH); break;
        case '=': 
            if (match('=')) addToken(tokens, TokenType::EQ_EQ);
            else addToken(tokens, TokenType::EQ); 
            break;
        case '<': 
            if (match('<')) addToken(tokens, TokenType::SHL);
            else addToken(tokens, TokenType::LESS);
            break;
        case '>': 
            if (match('>')) addToken(tokens, TokenType::SHR);
            break;
        case '&': addToken(tokens, TokenType::AMPERSAND); break;
        case '|': addToken(tokens, TokenType::PIPE); break;
        case '^': addToken(tokens, TokenType::CARET); break;
        case ' ': case '\r': case '\t': break;
        case '\n': line++; break;
        default:
            if (std::isdigit(c)) {
                while (std::isdigit(peek())) advance();
                addToken(tokens, TokenType::NUMBER);
            } else if (std::isalpha(c)) {
                while (std::isalnum(peek())) advance();
                std::string text = source.substr(start, current - start);
                TokenType type = TokenType::IDENTIFIER;
                if (text == "let") type = TokenType::LET;
                else if (text == "if") type = TokenType::IF;
                else if (text == "else") type = TokenType::ELSE;
                else if (text == "while") type = TokenType::WHILE;
                else if (text == "print") type = TokenType::PRINT;
                else if (text == "input") type = TokenType::INPUT;
                addToken(tokens, type);
            }
            break;
    }
}