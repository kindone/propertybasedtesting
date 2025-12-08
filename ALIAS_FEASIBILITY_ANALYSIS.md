# Feasibility Analysis: Creating C++-style Aliases in Python Gen Class

## Summary

**YES, we can create aliases for all C++ generator/combinator names in Python without breaking keyword restrictions.**

All C++ names are safe to use as Python method names. The main considerations are:
1. **Naming convention differences** (camelCase vs snake_case)
2. **Built-in name shadowing** (acceptable for method names)
3. **Implementation feasibility** (some require new functionality)

## Detailed Analysis

### ✅ Safe to Alias (No Conflicts)

#### Generators - Primitive Types
| C++ Name | Python Status | Notes |
|----------|---------------|-------|
| `gen::boolean` | ✅ Safe | Not a keyword, not a builtin. Can alias to `Gen.bool()` |
| `gen::character` | ✅ Safe | Not a keyword, not a builtin |
| `gen::int8`, `gen::uint8` | ✅ Safe | Not keywords, not builtins |
| `gen::int16`, `gen::uint16` | ✅ Safe | Not keywords, not builtins |
| `gen::int32`, `gen::uint32` | ✅ Safe | Not keywords, not builtins |
| `gen::int64`, `gen::uint64` | ✅ Safe | Not keywords, not builtins |
| `gen::integer` | ✅ Safe | Not a keyword, not a builtin |
| `gen::float32`, `gen::float64` | ✅ Safe | Not keywords, not builtins |
| `gen::arbitrary`, `gen::arbi` | ✅ Safe | Not keywords, not builtins (but need template-like API) |

#### Generators - Strings
| C++ Name | Python Status | Notes |
|----------|---------------|-------|
| `gen::string` | ✅ Safe | Not a keyword, not a builtin (Python uses `str` which is a builtin but works) |
| `gen::utf8string` | ✅ Safe | Not a keyword, not a builtin |
| `gen::utf16bestring` | ✅ Safe | Not a keyword, not a builtin |
| `gen::utf16lestring` | ✅ Safe | Not a keyword, not a builtin |
| `gen::cesu8string` | ✅ Safe | Not a keyword, not a builtin |

#### Generators - Containers
| C++ Name | Python Status | Notes |
|----------|---------------|-------|
| `gen::vector` | ✅ Safe | Not a keyword, not a builtin |
| `gen::map` | ✅ Safe | Builtin `map` exists, but method names can shadow builtins (Python already does this with `dict`, `set`, `list`, `tuple`) |
| `gen::optional` | ✅ Safe | Not a keyword, not a builtin |
| `gen::shared_ptr` | ✅ Safe | Not a keyword, not a builtin (underscore is fine) |
| `gen::pair` | ✅ Safe | Not a keyword, not a builtin |

#### Combinators
| C++ Name | Python Status | Notes |
|----------|---------------|-------|
| `gen::dependency` | ✅ Safe | Not a keyword, not a builtin |
| `gen::derive` | ✅ Safe | Not a keyword, not a builtin |
| `gen::intervals` | ✅ Safe | Not a keyword, not a builtin |
| `gen::uintervals` | ✅ Safe | Not a keyword, not a builtin |
| `gen::reference` | ✅ Safe | Not a keyword, not a builtin |

### ⚠️ Naming Convention Differences (camelCase vs snake_case)

These C++ names use camelCase, while Python uses snake_case. We can add both:

| C++ Name (camelCase) | Python Name (snake_case) | Can Add Both? |
|----------------------|--------------------------|---------------|
| `gen::oneOf` | `Gen.one_of` | ✅ Yes - add `oneOf` as alias |
| `gen::unionOf` | ❌ Not in Python | ✅ Yes - add `unionOf` (or `union_of`) |
| `gen::elementOf` | `Gen.element_of` | ✅ Yes - add `elementOf` as alias |
| `gen::weightedGen` | `Gen.weighted_gen` | ✅ Yes - add `weightedGen` as alias |
| `gen::weightedVal` | `Gen.weighted_value` | ✅ Yes - add `weightedVal` as alias |

**Note**: Python already uses builtin names as method names (`dict`, `set`, `list`, `tuple`, `str`, `int`, `float`, `bool`), so shadowing is acceptable.

### 🔍 Built-in Name Shadowing

Python already shadows builtins in the `Gen` class:
- `Gen.int()` - shadows builtin `int`
- `Gen.float()` - shadows builtin `float`
- `Gen.str()` - shadows builtin `str`
- `Gen.bool()` - shadows builtin `bool`
- `Gen.list()` - shadows builtin `list`
- `Gen.dict()` - shadows builtin `dict`
- `Gen.set()` - shadows builtin `set`
- `Gen.tuple()` - shadows builtin `tuple`

Therefore, adding `Gen.map()` would be consistent with existing practice.

### 📋 Implementation Requirements

#### Simple Aliases (Just Point to Existing Methods)
These can be simple assignments:
```python
# In Gen class
boolean = bool  # Alias for Gen.bool()
string = str    # Alias for Gen.str()
integer = int   # Alias for Gen.int()
oneOf = one_of  # Alias for Gen.one_of()
elementOf = element_of  # Alias for Gen.element_of()
weightedGen = weighted_gen  # Alias for Gen.weighted_gen()
weightedVal = weighted_value  # Alias for Gen.weighted_value()
```

#### New Functionality Required
These would need new implementations:

1. **Type-specific integer generators** (`int8`, `uint8`, `int16`, `uint16`, `int32`, `uint32`, `int64`, `uint64`)
   - Python doesn't have separate types, but we can create generators that produce values in those ranges
   - Example: `Gen.int8()` → generates values in range [-128, 127]

2. **Float32/Float64** (`float32`, `float64`)
   - Python `float` is always 64-bit, but we can add aliases
   - `Gen.float32()` → alias to `Gen.float()` (same implementation)
   - `Gen.float64()` → alias to `Gen.float()` (same implementation)

3. **UTF-16 strings** (`utf16bestring`, `utf16lestring`)
   - Would need new generator implementations

4. **CESU-8 string** (`cesu8string`)
   - Would need new generator implementation

5. **Vector** (`vector`)
   - Can alias to `Gen.list()` (same concept)

6. **Map** (`map`)
   - Can alias to `Gen.dict()` (same concept)

7. **Optional** (`optional`)
   - Would need new generator implementation (Python has `Optional[T]` type hint)

8. **Shared Pointer** (`shared_ptr`)
   - Python doesn't have shared_ptr, but could generate objects that are reference-counted
   - May not be meaningful in Python context

9. **Pair** (`pair`)
   - Can use `Gen.tuple()` with 2 elements, or create convenience method

10. **Arbitrary/Arbi** (`arbitrary`, `arbi`)
    - C++ uses templates, Python would need type parameter or different API
    - Could be: `Gen.arbitrary(int)` or `Gen[int]` (requires `__class_getitem__`)

11. **Dependency** (`dependency`)
    - Similar to `chain`, but returns `pair` instead of `tuple`
    - Could alias to `chain` or implement separately

12. **Derive** (`derive`)
    - Would need new implementation (similar to dependency but different semantics)

13. **Intervals** (`intervals`, `uintervals`)
    - Would need new implementation for interval-based integer generation

14. **Reference** (`reference`)
    - Would need new implementation (C++-specific concept)

15. **UnionOf** (`unionOf`)
    - Can alias to `Gen.one_of()` (same concept)

## Recommended Approach

### Phase 1: Simple Aliases (No New Code)
Add aliases that just point to existing methods:
- `boolean` → `bool`
- `string` → `str`
- `integer` → `int`
- `oneOf` → `one_of`
- `elementOf` → `element_of`
- `weightedGen` → `weighted_gen`
- `weightedVal` → `weighted_value`
- `unionOf` → `one_of`
- `vector` → `list`
- `map` → `dict`
- `float32` → `float`
- `float64` → `float`

### Phase 2: Type-Specific Generators (Simple Wrappers)
Add integer type generators that wrap `IntGenerator` with appropriate ranges:
- `int8()` → `IntGenerator(-128, 127)`
- `uint8()` → `IntGenerator(0, 255)`
- `int16()` → `IntGenerator(-32768, 32767)`
- `uint16()` → `IntGenerator(0, 65535)`
- `int32()` → `IntGenerator(-2147483648, 2147483647)`
- `uint32()` → `IntGenerator(0, 4294967295)`
- `int64()` → `IntGenerator(-9223372036854775808, 9223372036854775807)`
- `uint64()` → `IntGenerator(0, 18446744073709551615)`

### Phase 3: New Functionality (If Needed)
Implement generators that don't exist yet:
- UTF-16 string generators
- CESU-8 string generator
- Optional generator
- Pair generator (or document using tuple)
- Dependency/Derive combinators
- Intervals combinator
- Reference combinator

## Conclusion

**All C++ names can be safely aliased in Python.** There are no keyword conflicts, and builtin shadowing is already an established pattern in the `Gen` class. The main work is:

1. **Immediate**: Add simple aliases for existing functionality
2. **Short-term**: Add type-specific integer generators (simple wrappers)
3. **Long-term**: Implement new generators/combinators that don't exist yet (if desired)

The naming convention differences (camelCase vs snake_case) can be handled by providing both names, which is common in Python libraries that want to support multiple naming styles.

