#include "parser.h"
#include <cmath>

Parser::Parser(const std::string& input)
    : currentToken_(TokenType::End, 0.0, 0), tokenizer_(input), depth_(0) {
    advance(); // Read the first token
}

void Parser::advance() {
    currentToken_ = tokenizer_.nextToken();
}

void Parser::expect(TokenType type, const std::string& message) {
    if (currentToken_.type != type) {
        throw std::runtime_error(message);
    }
    advance();
}

double Parser::parse() {
    double result = parseExpression();

    if (currentToken_.type != TokenType::End) {
        throw std::runtime_error(
            "Unexpected token at position " + std::to_string(currentToken_.position));
    }

    return result;
}

// Expression → Term (('+' | '-') Term)*
double Parser::parseExpression() {
    double left = parseTerm();

    while (currentToken_.type == TokenType::Plus ||
           currentToken_.type == TokenType::Minus) {

        TokenType op = currentToken_.type;
        size_t opPos = currentToken_.position;
        advance();

        if (currentToken_.type == TokenType::End) {
            throw std::runtime_error(
                std::string("Expected number after '") +
                (op == TokenType::Plus ? "+" : "-") +
                "' at position " + std::to_string(opPos));
        }

        double right = parseTerm();

        if (op == TokenType::Plus) {
            left = left + right;
        } else {
            left = left - right;
        }
    }

    return left;
}

// Term → Factor (('*' | '/') Factor)*
double Parser::parseTerm() {
    double left = parseFactor();

    while (currentToken_.type == TokenType::Multiply ||
           currentToken_.type == TokenType::Divide) {

        TokenType op = currentToken_.type;
        size_t opPos = currentToken_.position;
        advance();

        if (currentToken_.type == TokenType::End) {
            throw std::runtime_error(
                std::string("Expected number after '") +
                (op == TokenType::Multiply ? "*" : "/") +
                "' at position " + std::to_string(opPos));
        }

        double right = parseFactor();

        if (op == TokenType::Multiply) {
            left = left * right;
        } else {
            if (right == 0.0) {
                throw std::runtime_error(
                    "Division by zero at position " + std::to_string(opPos));
            }
            left = left / right;
        }
    }

    return left;
}

// Factor → NUMBER | '(' Expression ')'
double Parser::parseFactor() {
    if (currentToken_.type == TokenType::Number) {
        double value = currentToken_.value;
        advance();
        return value;
    }

    if (currentToken_.type == TokenType::LParen) {
        ++depth_;
        if (depth_ > MAX_NESTING_DEPTH) {
            throw std::runtime_error(
                "Maximum nesting depth exceeded (" +
                std::to_string(MAX_NESTING_DEPTH) + " levels)");
        }

        advance(); // consume '('

        if (currentToken_.type == TokenType::RParen) {
            throw std::runtime_error(
                "Expected number inside parentheses at position " +
                std::to_string(currentToken_.position));
        }

        double result = parseExpression();

        expect(TokenType::RParen,
               "Expected ')' at position " + std::to_string(currentToken_.position));

        --depth_;
        return result;
    }

    // Error: expected a number or '('
    if (currentToken_.type == TokenType::End) {
        throw std::runtime_error("Unexpected end of expression");
    }

    throw std::runtime_error(
        "Unexpected token at position " + std::to_string(currentToken_.position));
}
