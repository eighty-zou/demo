# demo

A four-operation expression calculator in C++17. Type math expressions and get
instant results — supports `+`, `-`, `*`, `/`, parentheses, and decimals.

```
> 3 + 5 * 2
13
> (1 + 2) * (3 + 4)
21
> 10 / 3
3.33333
```

## Building

Requires CMake 3.14+ and a C++17 compiler.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

## Running

```bash
./build/calc
```

Type expressions at the `>` prompt. Type `quit` (case-insensitive) or press Ctrl+D to exit.

## Testing

```bash
ctest --test-dir build --output-on-failure
```

31 tests covering tokenizer, parser, operator precedence, associativity, parentheses, and error handling.

## Project Structure

```
src/
  main.cpp          REPL interface (prompt, input, output)
  tokenizer.h/.cpp  Lexer — converts input string to tokens
  parser.h/.cpp     Recursive descent parser with inline evaluation
tests/
  test_calculator.cpp  Unit tests (custom framework, no dependencies)
```

## Design Decisions

- **Recursive descent parser** with grammar: Expression → Term (`+`/`-`), Term → Factor (`*`/`/`), Factor → Number | `(` Expression `)`
- **Inline evaluation** — parser computes the result directly during parsing (no AST)
- **Division by zero** detected before every `/` operation with position-aware error messages
- **Nesting depth limit** of 100 levels to prevent stack overflow
- **Strict compiler warnings**: `-Wall -Wextra -Wpedantic -Werror`
- **AddressSanitizer** verified: zero memory errors
