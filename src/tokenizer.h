#pragma once

#include <string>
#include <stdexcept>

enum class TokenType {
    Number,
    Plus,
    Minus,
    Multiply,
    Divide,
    LParen,
    RParen,
    End
};

struct Token {
    TokenType type;
    double value;       // Only meaningful for Number tokens
    size_t position;    // Position in the input string

    Token(TokenType t, double v, size_t pos)
        : type(t), value(v), position(pos) {}
};

class Tokenizer {
public:
    explicit Tokenizer(const std::string& input);

    Token nextToken();

private:
    void skipWhitespace();
    Token readNumber();

    std::string input_;
    size_t pos_;
};
