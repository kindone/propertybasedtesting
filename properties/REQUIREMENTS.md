# Requirements: Fundamental Properties Documentation

## Purpose

Document fundamental reusable properties for stateful testing. These properties apply across many stateful components and serve as templates for deriving property-based tests.

## Format Requirements

### Must Include

1. **Property statement**: One-line description of what the property asserts
2. **Concept section**: 
   - What to test (what we're verifying)
   - How to think about it (the approach)
   - Why it matters (key idea)
3. **Actions section**: Description of what kinds of actions/operations to generate
   - State-modifying operations
   - Edge case operations
   - Operations specific to the property
4. **Checks section**: Description of what to verify
   - What invariants/conditions to check
   - When to check (after each action, after sequence, etc.)
   - Component-specific validity checks
5. **Subdomains section**: Practical categorization of subdomains to explore
   - Use simple, practical categories (e.g., ideal conditions, scenarios requiring handling, extreme scenarios)
   - Or property-specific categories (e.g., no failures, single failures, multiple failures)
   - Keep minimal and thought-provoking

### Must NOT Include

- Actual code (C++, pseudocode, or any implementation)
- Generator definitions (assume generators are defined elsewhere)
- Multiple examples (one concept is enough unless distinct approaches are needed)
- Verbose explanations or implementation notes
- Hyperparameters (removed - focus on subdomains instead)

### File Structure

Each property file should follow this structure:

```markdown
# [Property Name] Property

**Property**: [One-line statement]

## Concept

[What to test, how to think about it, why it matters]

**Key idea**: [Core insight]

## Actions

[Description of actions/operations to generate]

## Checks

[Description of what to verify]

## Subdomains

[Practical categorization - keep minimal and thought-provoking]
```

## Properties to Document

1. ✅ **Crash tolerance** - Component doesn't crash on any action sequence
2. ✅ **Malfunction tolerance** - Component handles erroneous responses from dependencies gracefully
3. ⏳ **Exception-safety** - Exceptions don't leave component in invalid state
4. ⏳ **Memory safety** - No leaks, double-free, use-after-free
5. ⏳ **Thread safety** - Safe concurrent access
6. ⏳ **Resource management** - Resources acquired are released
7. ⏳ **Observability** - State queries return correct values
8. ⏳ **Consistency** - Internal invariants maintained

## Principles

- **Concept-first**: Focus on the concept, not implementation
- **Minimal**: Keep files concise and scannable
- **Reusable**: Properties should apply across many components
- **Action-oriented**: Clearly describe what actions to generate
- **Check-oriented**: Clearly describe what to verify
- **Subdomain-focused**: Practical categorization, not exhaustive lists

## Naming Convention

Files should be named: `NN_property_name.md` where NN is a two-digit number (01, 02, etc.) and property_name is lowercase with underscores.

## References

- See `../propertyderivation.md` for the methodology behind these properties
- See `../subdomainexploration.md` for subdomain exploration strategies
- See `../property_derivation_prompt.md` for LLM guidance on deriving properties
