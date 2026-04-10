# AGENTS.md

## Project Overview
Carpio is developed with modern C++.

## Core Principles
- Use modern C++ style and idioms when adding or modifying code.
- Prefer clear, type-safe, maintainable C++ over legacy-style patterns unless compatibility requires otherwise.
- Use English for code, comments, and documentation throughout the project.
- Keep the algorithmic implementation header-only.
- In the `include/` directory, C++ sources are treated as header-only implementation files.
- Do not move core algorithm logic into `.cpp` files unless explicitly requested.

## Implementation Guidance
- When extending algorithm modules, preserve the existing header-only organization.
- Prefer templates, inline functions, and header-defined implementations where appropriate.
- Keep new code consistent with the current project structure and naming style.
- Avoid introducing unnecessary runtime or build-system complexity.

## Change Discipline
- Prefer small, local changes over broad refactors.
- Preserve existing public behavior unless the task explicitly requires a behavior change.
- If a modernization change risks altering behavior, keep the behavior first and modernize conservatively.

## Future Additions
Additional project rules may be added here for:
- naming conventions
- testing expectations
- refactoring boundaries
- documentation style
