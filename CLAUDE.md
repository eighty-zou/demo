## fstack (Claude mode)

Model: Claude (Sonnet / Opus / Haiku via Claude Code)
Available skills: /plan-ceo-review, /plan-eng-review, /plan-design-review,
/design-consultation, /review, /ship, /qa, /qa-only, /design-review,
/retro, /document-release
Project type detected by `fstack-detect-language` — supports C, C++, and mixed projects

### Claude features in effect:

**Interactive by default.** All fstack skills use `AskUserQuestion` at decision
points. Claude pauses, shows options with completeness scores, and waits for your
direction before proceeding with consequential changes.

**Extended thinking for complex analysis.** `/plan-eng-review`, `/design-review`,
and `/review` use Claude's extended thinking budget for deeper analysis of ownership
models, API design, and memory safety. Expect more thorough output and slightly
longer run times on complex codebases.

**Full tool suite.** All fstack tools are available: Bash, Read, Edit, Glob, Grep,
Write, AskUserQuestion. No restrictions on file count or session length.

**Completeness over speed.** Claude defaults to the Boil the Lake principle: if
the complete implementation is achievable in a single session, do it. Don't recommend
shortcuts when the full solution is within reach.

### C++ Design Principles (active)

- **YAGNI** (Priority 1): Build for today's requirements. No hypothetical abstractions.
- **KISS** (Priority 2): Simplest solution that works. Prefer `if constexpr` over SFINAE.
- **DRY** (Priority 3): One authoritative source per piece of knowledge.
- **SOLID** (Priority 4): SRP, OCP, LSP, ISP, DIP. No god classes or singletons.
