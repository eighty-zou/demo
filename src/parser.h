#pragma once

#include "tokenizer.h"
#include <string>

// Maximum nesting depth for parentheses.
// Prevents stack overflow from deeply nested expressions like (((((...)))))
constexpr int MAX_NESTING_DEPTH = 100;

class Parser {
public:
    explicit Parser(const std::string& input);

    // Parse and evaluate the expression. Returns the result.
    // Throws std::runtime_error on parse errors or division by zero.
    double parse();

private:
    double parseExpression();
    double parseTerm();
    double parseFactor();

    Token currentToken_;
    Tokenizer tokenizer_;
    int depth_;

    void advance();
    void expect(TokenType type, const std::string& message);
};
