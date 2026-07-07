# TODOS

## Future: Migrate to AST-based parser when adding variables or functions

**What:** Refactor the parser from inline evaluation to building an Abstract Syntax Tree (AST), then evaluate it in a separate pass.

**Why:** The current design evaluates expressions directly during parsing — the parser computes the answer as it reads tokens. This works for simple arithmetic, but the moment you add variables (`x = 5; x + 3`) or functions (`sin(x)`), you need to store and look up values, which requires a tree structure you can traverse multiple times.

**Pros:** Enables variables, functions, and multi-pass evaluation. Cleaner separation of parsing and evaluation logic.

**Cons:** Adds ~80 lines and a new module (`evaluator.h/.cpp`). Significant refactor of `parser.cpp`.

**Context:** The current parser uses recursive descent with inline evaluation: `parseExpression()` returns a `double` directly. The refactor would change it to return an AST node (e.g., `struct ExprNode { enum Type { Number, BinOp, Var, Func }; ... }`), and add a separate `evaluate(ExprNode&)` function. The tokenizer stays unchanged.

**Depends on:** Adding a feature that requires stored state (variables, named functions).

**Blocked by:** Nothing — can be done anytime.

---
