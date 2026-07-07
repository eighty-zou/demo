#include "tokenizer.h"
#include "parser.h"
#include <iostream>
#include <cmath>
#include <string>
#include <stdexcept>

// Minimal test framework — no external dependencies needed
static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

#define TEST(name) \
    static void test_##name(); \
    struct Register_##name { \
        Register_##name() { \
            tests_run++; \
            std::cout << "  " << #name << "... "; \
            try { \
                test_##name(); \
                tests_passed++; \
                std::cout << "PASS" << std::endl; \
            } catch (const std::exception& e) { \
                tests_failed++; \
                std::cout << "FAIL: " << e.what() << std::endl; \
            } \
        } \
    } register_##name; \
    static void test_##name()

#define ASSERT_EQ(expected, actual) \
    if (std::abs((expected) - (actual)) > 1e-9) { \
        throw std::runtime_error( \
            std::string("Expected ") + std::to_string(expected) + \
            " but got " + std::to_string(actual)); \
    }

#define ASSERT_THROWS(expr) \
    { \
        bool threw = false; \
        try { expr; } catch (const std::runtime_error&) { threw = true; } \
        if (!threw) { \
            throw std::runtime_error("Expected exception but none was thrown"); \
        } \
    }

#define ASSERT_THROWS_MSG(expr, substring) \
    { \
        bool threw = false; \
        std::string msg; \
        try { expr; } catch (const std::runtime_error& e) { threw = true; msg = e.what(); } \
        if (!threw) { \
            throw std::runtime_error("Expected exception but none was thrown"); \
        } \
        if (msg.find(substring) == std::string::npos) { \
            throw std::runtime_error( \
                std::string("Expected error containing '") + substring + \
                "' but got: " + msg); \
        } \
    }

// ============================================================
// TOKENIZER TESTS
// ============================================================

TEST(tokenize_single_number) {
    Tokenizer t("42");
    Token tok = t.nextToken();
    ASSERT_EQ(42.0, tok.value);
    if (tok.type != TokenType::Number) throw std::runtime_error("Expected Number token");
}

TEST(tokenize_decimal) {
    Tokenizer t("3.14");
    Token tok = t.nextToken();
    ASSERT_EQ(3.14, tok.value);
}

TEST(tokenize_multiple_decimals) {
    Tokenizer t("3.14.15");
    ASSERT_THROWS(t.nextToken());
}

TEST(tokenize_operators) {
    Tokenizer t("+-*/");
    if (t.nextToken().type != TokenType::Plus) throw std::runtime_error("Expected Plus");
    if (t.nextToken().type != TokenType::Minus) throw std::runtime_error("Expected Minus");
    if (t.nextToken().type != TokenType::Multiply) throw std::runtime_error("Expected Multiply");
    if (t.nextToken().type != TokenType::Divide) throw std::runtime_error("Expected Divide");
}

TEST(tokenize_parens) {
    Tokenizer t("()");
    if (t.nextToken().type != TokenType::LParen) throw std::runtime_error("Expected LParen");
    if (t.nextToken().type != TokenType::RParen) throw std::runtime_error("Expected RParen");
}

TEST(tokenize_end) {
    Tokenizer t("");
    Token tok = t.nextToken();
    if (tok.type != TokenType::End) throw std::runtime_error("Expected End token");
}

TEST(tokenize_skips_whitespace) {
    Tokenizer t("  3  +  5  ");
    Token t1 = t.nextToken();
    Token t2 = t.nextToken();
    Token t3 = t.nextToken();
    ASSERT_EQ(3.0, t1.value);
    if (t2.type != TokenType::Plus) throw std::runtime_error("Expected Plus");
    ASSERT_EQ(5.0, t3.value);
}

TEST(tokenize_invalid_char) {
    Tokenizer t("abc");
    ASSERT_THROWS(t.nextToken());
}

TEST(tokenize_position_tracking) {
    Tokenizer t("3 + 5");
    Token t1 = t.nextToken();
    Token t2 = t.nextToken();
    Token t3 = t.nextToken();
    if (t1.position != 0) throw std::runtime_error("Expected position 0");
    if (t2.position != 2) throw std::runtime_error("Expected position 2");
    if (t3.position != 4) throw std::runtime_error("Expected position 4");
}

// ============================================================
// PARSER TESTS — Happy Path
// ============================================================

TEST(parse_single_number) {
    ASSERT_EQ(42.0, Parser("42").parse());
}

TEST(parse_addition) {
    ASSERT_EQ(8.0, Parser("3 + 5").parse());
}

TEST(parse_subtraction) {
    ASSERT_EQ(7.0, Parser("10 - 3").parse());
}

TEST(parse_multiplication) {
    ASSERT_EQ(12.0, Parser("4 * 3").parse());
}

TEST(parse_division) {
    ASSERT_EQ(2.5, Parser("10 / 4").parse());
}

TEST(parse_precedence_multiply_before_add) {
    ASSERT_EQ(13.0, Parser("3 + 5 * 2").parse());
}

TEST(parse_precedence_divide_before_subtract) {
    ASSERT_EQ(7.0, Parser("10 - 6 / 2").parse());
}

TEST(parse_parentheses_override) {
    ASSERT_EQ(16.0, Parser("(3 + 5) * 2").parse());
}

TEST(parse_nested_parentheses) {
    ASSERT_EQ(10.0, Parser("((2 + 3)) * 2").parse());
}

TEST(parse_complex_expression) {
    ASSERT_EQ(21.0, Parser("(1 + 2) * (3 + 4)").parse());
}

TEST(parse_left_to_right_add_sub) {
    // 10 - 3 - 2 = 5 (not 9)
    ASSERT_EQ(5.0, Parser("10 - 3 - 2").parse());
}

TEST(parse_left_to_right_mul_div) {
    // 12 / 3 * 2 = 8 (not 2)
    ASSERT_EQ(8.0, Parser("12 / 3 * 2").parse());
}

TEST(parse_decimal_arithmetic) {
    ASSERT_EQ(6.28, Parser("3.14 * 2").parse());
}

TEST(parse_whitespace_variations) {
    ASSERT_EQ(8.0, Parser("  3  +  5  ").parse());
}

// ============================================================
// PARSER TESTS — Error Cases
// ============================================================

TEST(error_trailing_operator) {
    ASSERT_THROWS(Parser("3 +").parse());
}

TEST(error_missing_closing_paren) {
    ASSERT_THROWS(Parser("(3 + 5").parse());
}

TEST(error_double_operator) {
    ASSERT_THROWS(Parser("3 + + 5").parse());
}

TEST(error_division_by_zero) {
    ASSERT_THROWS_MSG(Parser("3 / 0").parse(), "Division by zero");
}

TEST(error_invalid_character) {
    ASSERT_THROWS(Parser("abc").parse());
}

TEST(error_empty_parentheses) {
    ASSERT_THROWS(Parser("()").parse());
}

TEST(error_deep_nesting) {
    // Build 101 levels of nesting
    std::string input;
    for (int i = 0; i < 101; ++i) input += "(";
    input += "1";
    for (int i = 0; i < 101; ++i) input += ")";
    ASSERT_THROWS(Parser(input).parse());
}

TEST(ok_100_levels_nesting) {
    // 100 levels should work fine
    std::string input;
    for (int i = 0; i < 100; ++i) input += "(";
    input += "1";
    for (int i = 0; i < 100; ++i) input += ")";
    ASSERT_EQ(1.0, Parser(input).parse());
}

// ============================================================
// main
// ============================================================

int main() {
    std::cout << std::endl;
    std::cout << "==============================" << std::endl;
    std::cout << "Calculator Test Results" << std::endl;
    std::cout << "==============================" << std::endl;
    std::cout << std::endl;
    std::cout << "Tests:" << std::endl;

    // Tests already ran during static initialization above.
    // Just print the summary:

    std::cout << std::endl;
    std::cout << "==============================" << std::endl;
    std::cout << "Total:  " << tests_run << std::endl;
    std::cout << "Passed: " << tests_passed << std::endl;
    std::cout << "Failed: " << tests_failed << std::endl;
    std::cout << "==============================" << std::endl;

    return tests_failed > 0 ? 1 : 0;
}
