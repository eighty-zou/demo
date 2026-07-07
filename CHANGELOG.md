# Changelog

All notable changes to this project will be documented in this file.

## [1.0.0.0] - 2026-07-07

### Added
- Type math expressions at a REPL prompt — supports `+`, `-`, `*`, `/`, decimals, and parentheses
- Operator precedence works as expected: `3 + 5 * 2 = 13`, not `16`
- Left-to-right associativity: `10 - 3 - 2 = 5`, not `9`
- Nest parentheses up to 100 levels deep: `((2 + 3)) * 2 = 10`
- Clear error messages with position info when you mistype an expression or divide by zero
- Handles whitespace flexibly — `3+5` and `  3  +  5  ` both work
- Interactive REPL with `quit` command (any case) and clean EOF exit (Ctrl+D)
- 31 unit tests covering happy paths, error cases, and edge cases (deep nesting, multiple decimals)
- CMake build with zero warnings under `-Wall -Wextra -Wpedantic -Werror`
- AST migration path documented in TODOS.md for future variable/function support
