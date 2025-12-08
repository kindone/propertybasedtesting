# Generator Namespace Comparison: C++ vs Python

This document compares the `gen` namespace in C++Proptest with the `Gen` class in Python-Proptest.

## Generators

### Primitive Types

| Feature | C++ (`gen::`) | Python (`Gen.`) | Notes |
|---------|---------------|-----------------|-------|
| **Boolean** | `gen::boolean` | `Gen.bool(true_prob=0.5)` | Python allows probability control |
| **Character** | `gen::character` | `Gen.ascii_char()`, `Gen.printable_ascii_char()`, `Gen.unicode_char()` | Python has multiple variants |
| **Integers** | | | |
| - 8-bit | `gen::int8`, `gen::uint8` | ❌ | Python uses single `int` type |
| - 16-bit | `gen::int16`, `gen::uint16` | ❌ | Python uses single `int` type |
| - 32-bit | `gen::int32`, `gen::uint32` | ❌ | Python uses single `int` type |
| - 64-bit | `gen::int64`, `gen::uint64` | ❌ | Python uses single `int` type |
| - Generic | `gen::integer` | `Gen.int(min_value, max_value)` | Python allows range specification |
| - Interval | ❌ | `Gen.interval(min, max)` | Python convenience method |
| - In Range | ❌ | `Gen.in_range(min, max)` | Python exclusive range |
| - Integers (alias) | ❌ | `Gen.integers(min, max)` | Python alias for interval |
| **Floating Point** | | | |
| - 32-bit | `gen::float32` | ❌ | Python `float` is 64-bit |
| - 64-bit | `gen::float64` | `Gen.float(min_value, max_value, nan_prob, posinf_prob, neginf_prob)` | Python has special value control |
| **Arbitrary Type** | `gen::arbitrary<T>`, `gen::arbi<T>` | ❌ | C++ template-based |

### String Generators

| Feature | C++ (`gen::`) | Python (`Gen.`) | Notes |
|---------|---------------|-----------------|-------|
| **Basic String** | `gen::string` | `Gen.str(min_length, max_length, charset)` | Python allows charset control |
| **UTF-8 String** | `gen::utf8string` | `Gen.unicode_string(min_length, max_length)` | |
| **UTF-16 String** | `gen::utf16bestring`, `gen::utf16lestring` | ❌ | C++ has BE/LE variants |
| **CESU-8 String** | `gen::cesu8string` | ❌ | C++ only |
| **ASCII String** | ❌ | `Gen.ascii_string(min_length, max_length)` | Python convenience |
| **Printable ASCII** | ❌ | `Gen.printable_ascii_string(min_length, max_length)` | Python convenience |

### Container Generators

| Feature | C++ (`gen::`) | Python (`Gen.`) | Notes |
|---------|---------------|-----------------|-------|
| **List/Vector** | `gen::vector<T>`, `gen::list<T>` | `Gen.list(element_gen, min_length, max_length)` | Python allows length constraints |
| **Unique List** | ❌ | `Gen.unique_list(element_gen, min_length, max_length)` | Python only |
| **Set** | `gen::set<T>` | `Gen.set(element_gen, min_size, max_size)` | Python allows size constraints |
| **Map/Dict** | `gen::map<Key, Value>` | `Gen.dict(key_gen, value_gen, min_size, max_size)` | Python allows size constraints |
| **Optional** | `gen::optional<T>` | ❌ | C++ only |
| **Shared Pointer** | `gen::shared_ptr<T>` | ❌ | C++ only |
| **Pair** | `gen::pair<T1, T2>()`, `gen::pair<T1, T2>(gen1, gen2)` | ❌ | C++ only (Python uses tuple) |
| **Tuple** | `gen::tuple<ARGS...>()`, `gen::tuple<ARGS...>(gens...)` | `Gen.tuple(*generators)` | Both support variadic |

## Combinators

### Basic Combinators

| Feature | C++ (`gen::`) | Python (`Gen.`) | Notes |
|---------|---------------|-----------------|-------|
| **Just** | `gen::just<T>(value)` | `Gen.just(value)` | Generate constant value |
| **Lazy** | `gen::lazy<T>(func)` | `Gen.lazy(func)` | Lazy evaluation |
| **One Of** | `gen::oneOf<T>(gens...)`, `gen::unionOf<T>(gens...)` | `Gen.one_of(*generators)` | Choose from generators |
| **Element Of** | `gen::elementOf<T>(values...)` | `Gen.element_of(*values)` | Choose from values |
| **Weighted Generator** | `gen::weightedGen<T>(gen, weight)` | `Gen.weighted_gen(generator, weight)` | Weight for oneOf |
| **Weighted Value** | `gen::weightedVal<T>(value, weight)` | `Gen.weighted_value(value, weight)` | Weight for elementOf |

### Transform Combinators

| Feature | C++ (`gen::`) | Python (`Gen.`) | Notes |
|---------|---------------|-----------------|-------|
| **Map/Transform** | `gen::transform(gen, func)` | `generator.map(func)` | Method on Generator |
| **Filter** | `gen::filter(gen, predicate)` | `generator.filter(predicate)` | Method on Generator |
| **FlatMap** | ❌ | `generator.flat_map(func)` | Python only (method) |

### Dependency Combinators

| Feature | C++ (`gen::`) | Python (`Gen.`) | Notes |
|---------|---------------|-----------------|-------|
| **Chain** | `gen::chain(gen1, gen2gen)` | `Gen.chain(base_gen, gen_factory)` | Dependent generation |
| **Dependency** | `gen::dependency(gen1, gen2gen)` | ❌ | C++ only (similar to chain) |
| **Derive** | `gen::derive(gen1, gen2gen)` | ❌ | C++ only |
| **Aggregate** | `gen::aggregate(gen1, gen2gen, minSize, maxSize)` | `Gen.aggregate(initial_gen, gen_factory, min_size, max_size)` | List of dependent values |
| **Accumulate** | `gen::accumulate(gen1, gen2gen, minSize, maxSize)` | `Gen.accumulate(initial_gen, gen_factory, min_size, max_size)` | Final value from accumulation |

### Special Combinators

| Feature | C++ (`gen::`) | Python (`Gen.`) | Notes |
|---------|---------------|-----------------|-------|
| **Construct** | `gen::construct<Class>()`, `gen::construct<Class>(gens...)` | `Gen.construct(Type, *generators)` | Create class instances |
| **Intervals** | `gen::intervals(interval_list)`, `gen::uintervals(interval_list)` | ❌ | C++ only (integer intervals) |
| **Reference** | `gen::reference<T>(ref)` | ❌ | C++ only |

## Summary

### C++ Has, Python Doesn't:
- Type-specific integer generators (int8, int16, int32, int64, uint variants)
- UTF-16 string generators (BE/LE variants)
- CESU-8 string generator
- Optional and shared_ptr generators
- Pair generator (Python uses tuple instead)
- Dependency and Derive combinators
- Intervals combinator
- Reference combinator
- Template-based `arbitrary<T>` and `arbi<T>`

### Python Has, C++ Doesn't:
- `unique_list` generator
- ASCII string convenience methods (`ascii_string`, `printable_ascii_string`)
- Character generators (`ascii_char`, `printable_ascii_char`, `unicode_char`)
- `interval`, `in_range`, `integers` convenience methods
- `chain_tuple` (deprecated, use `chain`)
- `flat_map` combinator (as method)
- Probability control for boolean (`true_prob`)
- Special value control for floats (`nan_prob`, `posinf_prob`, `neginf_prob`)
- Charset control for strings

### Both Have (with differences):
- **Integers**: C++ has type-specific, Python has single `int` with range control
- **Floats**: C++ has float32/float64, Python has single `float` with special value control
- **Strings**: Both have basic and UTF-8, but different APIs
- **Containers**: Both have list/vector, set, map/dict, tuple
- **Combinators**: Both have just, lazy, oneOf, elementOf, chain, aggregate, accumulate, construct

## API Style Differences

1. **C++**: Template-based, type parameters in angle brackets
2. **Python**: Method-based, type parameters in function arguments
3. **C++**: More low-level control (specific integer types, UTF-16 variants)
4. **Python**: More convenience methods and higher-level abstractions
5. **C++**: Uses `gen::` namespace
6. **Python**: Uses `Gen.` class with static methods


