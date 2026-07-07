#include "tokenizer.h"
#include <cctype>

Tokenizer::Tokenizer(const std::string& input)
    : input_(input), pos_(0) {}

void Tokenizer::skipWhitespace() {
    while (pos_ < input_.size() && std::isspace(static_cast<unsigned char>(input_[pos_]))) {
        ++pos_;
    }
}

Token Tokenizer::readNumber() {
    size_t start = pos_;
    bool hasDot = false;

    while (pos_ < input_.size() &&
           (std::isdigit(static_cast<unsigned char>(input_[pos_])) || input_[pos_] == '.')) {
        if (input_[pos_] == '.') {
            if (hasDot) {
                throw std::runtime_error(
                    "Invalid number: multiple decimal points at position " + std::to_string(start));
            }
            hasDot = true;
        }
        ++pos_;
    }

    std::string numStr = input_.substr(start, pos_ - start);
    try {
        double value = std::stod(numStr);
        return Token(TokenType::Number, value, start);
    } catch (const std::exception&) {
        throw std::runtime_error(
            "Invalid number '" + numStr + "' at position " + std::to_string(start));
    }
}

Token Tokenizer::nextToken() {
    skipWhitespace();

    if (pos_ >= input_.size()) {
        return Token(TokenType::End, 0.0, pos_);
    }

    char ch = input_[pos_];

    // Numbers
    if (std::isdigit(static_cast<unsigned char>(ch))) {
        return readNumber();
    }

    // Single-character tokens
    size_t tokenPos = pos_;
    ++pos_;

    switch (ch) {
        case '+': return Token(TokenType::Plus,     0.0, tokenPos);
        case '-': return Token(TokenType::Minus,    0.0, tokenPos);
        case '*': return Token(TokenType::Multiply, 0.0, tokenPos);
        case '/': return Token(TokenType::Divide,   0.0, tokenPos);
        case '(': return Token(TokenType::LParen,   0.0, tokenPos);
        case ')': return Token(TokenType::RParen,   0.0, tokenPos);
        default:
            throw std::runtime_error(
                std::string("Unexpected character '") + ch + "' at position " +
                std::to_string(tokenPos));
    }
}
