#pragma once
#include <string>
#include <cstdint>

enum class TokenType {
    PLUS, MINUS, STAR, SLASH, EQ, EQ_EQ, LESS,
    AMPERSAND, PIPE, CARET, LPAREN, RPAREN, LBRACE, RBRACE, SEMICOLON,
    SHL, SHR,
    NUMBER, IDENTIFIER,
    LET, IF, ELSE, WHILE, PRINT, INPUT,
    EOF_TOKEN, ERROR_TOKEN
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line;
};

enum OpCode : uint8_t {
    OP_CONSTANT,
    OP_GET_GLOBAL,
    OP_SET_GLOBAL,
    OP_ADD, OP_SUB, OP_MUL, OP_DIV,
    OP_EQ, OP_LESS,
    OP_BIT_AND, OP_BIT_OR, OP_BIT_XOR, OP_SHL, OP_SHR,
    OP_PRINT,
    OP_INPUT,
    OP_JUMP,
    OP_JUMP_IF_FALSE,
    OP_LOOP,
    OP_POP,
    OP_HALT
};