# Changelog

All notable changes to this project will be documented in this file.

## [1.0.0.0] - 2026-07-07

### Added
- Four-operation expression calculator (addition, subtraction, multiplication, division)
- Recursive descent parser with operator precedence (`*/÷` before `+/-`)
- Left-to-right associativity for all operators
- Parenthesized expressions with nesting support (up to 100 levels)
- Division by zero detection with position-aware error messages
- Tokenizer with position tracking, decimal number support, and whitespace handling
- REPL interface with `>` prompt, case-insensitive `quit` command, and EOF handling
- 31 unit tests covering tokenizer, parser happy path, and error cases
- CMake build system with strict warnings (`-Wall -Wextra -Wpedantic -Werror`)
- TODOS.md with AST migration path for future variable/function support
