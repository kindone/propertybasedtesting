# Feasibility Analysis: Creating C++-style Aliases in Python Gen Class

## Summary

**We should only alias truly interchangeable/equivalent names, not force C++ naming conventions into Python.**

Honoring language styles means:
- ✅ **Alias semantic equivalents**: `boolean`/`bool`, `integer`/`int`, `string`/`str` (same meaning, different word lengths)
- ❌ **Don't alias style differences**: `oneOf`/`one_of`, `elementOf`/`element_of` (camelCase vs snake_case)
- ❌ **Don't alias language-specific concepts**: `vector`/`list`, `map`/`dict` (different semantic contexts)

## Recommended Aliases (Semantic Equivalents Only)

### ✅ Safe and Appropriate Aliases

These are truly interchangeable names that mean the same thing:

| C++ Name | Python Name | Rationale |
|----------|-------------|-----------|
| `gen::boolean` | `Gen.bool` | Both mean "boolean type" - just different word lengths |
| `gen::integer` | `Gen.int` | Both mean "integer type" - just different word lengths |
| `gen::string` | `Gen.str` | Both mean "string type" - just different word lengths |

**Implementation:**
```python
# In Gen class - simple aliases
boolean = bool      # Alias for Gen.bool()
integer = int        # Alias for Gen.int()
string = str        # Alias for Gen.str()
```

### ❌ Should NOT Alias (Style Differences)

These are naming convention differences, not semantic equivalents:

| C++ Name | Python Name | Reason |
|----------|-------------|--------|
| `gen::oneOf` | `Gen.one_of` | camelCase vs snake_case - style difference |
| `gen::elementOf` | `Gen.element_of` | camelCase vs snake_case - style difference |
| `gen::weightedGen` | `Gen.weighted_gen` | camelCase vs snake_case - style difference |
| `gen::weightedVal` | `Gen.weighted_value` | camelCase vs snake_case - style difference |
| `gen::unionOf` | ❌ Not in Python | Would need to add, but should use `union_of` if added |

**Rationale**: Python uses snake_case consistently. Adding camelCase aliases would:
- Violate PEP 8 style guidelines
- Create inconsistency in the API
- Confuse Python developers expecting snake_case
- Not provide real value (just cosmetic differences)

### ❌ Should NOT Alias (Language-Specific Concepts)

These represent different concepts in different languages:

| C++ Name | Python Name | Reason |
|----------|-------------|--------|
| `gen::vector` | `Gen.list` | Different semantic contexts - C++ uses `vector`, Python uses `list` |
| `gen::map` | `Gen.dict` | Different semantic contexts - C++ uses `map`, Python uses `dict` |

**Rationale**: While they're conceptually similar (dynamic arrays, key-value stores), they're named differently in each language's standard library. Aliasing would:
- Confuse Python developers (who expect `list` and `dict`)
- Not match the language's idioms
- Create unnecessary cognitive overhead

### ❓ Context-Dependent (Type-Specific Generators)

These are C++-specific type concepts that don't exist in Python:

| C++ Name | Python Equivalent | Recommendation |
|----------|-------------------|----------------|
| `gen::int8`, `gen::uint8`, etc. | ❌ Not in Python | **Don't add** - Python doesn't have these as separate types. If needed, document using `Gen.int(min, max)` with appropriate ranges. |
| `gen::float32`, `gen::float64` | `Gen.float` | **Don't add** - Python `float` is always 64-bit. Adding `float32`/`float64` would be misleading. |
| `gen::optional` | ❌ Not in Python | **Consider adding** if useful, but use Python naming (`optional` is fine, or `opt`) |
| `gen::shared_ptr` | ❌ Not in Python | **Don't add** - Python doesn't have shared_ptr concept (everything is reference-counted) |
| `gen::pair` | `Gen.tuple` | **Don't add** - Python uses `tuple` for pairs. Document using `Gen.tuple(gen1, gen2)`. |

## Python Style Guidelines

### PEP 8 Compliance

Python follows PEP 8 naming conventions:
- **Functions and methods**: `snake_case` (e.g., `one_of`, `element_of`)
- **Classes**: `PascalCase` (e.g., `Gen`, `Generator`)
- **Constants**: `UPPER_SNAKE_CASE` (e.g., `MAX_SIZE`)

The current `Gen` class correctly uses `snake_case` for all methods.

### Consistency Matters

Looking at the existing codebase:
- ✅ `Gen.one_of()` - snake_case
- ✅ `Gen.element_of()` - snake_case
- ✅ `Gen.weighted_gen()` - snake_case
- ✅ `Gen.weighted_value()` - snake_case
- ✅ `Gen.chain()` - snake_case
- ✅ `Gen.aggregate()` - snake_case

Adding camelCase aliases would break this consistency.

## Final Recommendation

### ✅ Add These Aliases (3 total)

```python
class Gen:
    # ... existing methods ...
    
    # Semantic equivalent aliases
    boolean = bool      # Alias for Gen.bool()
    integer = int       # Alias for Gen.int()
    string = str        # Alias for Gen.str()
```

**Rationale**: These are truly interchangeable - same meaning, just different word lengths. They're common in both languages and don't violate style guidelines.

### ❌ Don't Add These

1. **Style differences** (camelCase vs snake_case):
   - `oneOf`, `elementOf`, `weightedGen`, `weightedVal`, `unionOf`
   - Reason: Violates Python style conventions

2. **Language-specific concepts**:
   - `vector` → `list`, `map` → `dict`
   - Reason: Different semantic contexts in each language

3. **Type-specific generators** (C++-only concepts):
   - `int8`, `uint8`, `int16`, `uint16`, `int32`, `uint32`, `int64`, `uint64`
   - `float32`, `float64`
   - `shared_ptr`
   - Reason: Don't exist in Python or would be misleading

4. **New functionality** (would need implementation):
   - `utf16bestring`, `utf16lestring`, `cesu8string`
   - `dependency`, `derive`, `intervals`, `reference`
   - Reason: Would need new implementations, not just aliases

## Conclusion

**Keep it minimal and style-appropriate.** Only add aliases for truly semantic equivalents (`boolean`/`bool`, `integer`/`int`, `string`/`str`). Don't add aliases that:
- Violate Python naming conventions (camelCase)
- Force C++ idioms into Python (vector/list, map/dict)
- Represent concepts that don't exist in Python (type-specific integers, shared_ptr)

This approach:
- ✅ Respects Python style (PEP 8)
- ✅ Maintains API consistency
- ✅ Provides value without confusion
- ✅ Keeps the codebase clean and maintainable
