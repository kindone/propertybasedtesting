# Generator Constructors with Named Parameters Plan

## Overview

Add named parameter support to generator constructors using C++20 designated initializers, similar to `ForAllConfig`. This will make generator APIs more readable, self-documenting, and less error-prone, especially for generators with multiple similar-typed parameters.

## Motivation

Many generators in cppproptest take multiple parameters that can be confusing when passed positionally:

1. **Floating point generators**: `Arbi<float>(0.1, 0.05, 0.02)` - Which probability is which?
2. **Container generators**: `Arbi<vector<int>>(gen::int32(), 5, 20)` - Is 5 minSize or maxSize?
3. **Optional generator**: `Arbi<optional<int>>(gen::int32(), 0.9)` - What does 0.9 mean?

Named parameters solve these issues by making the code self-documenting.

## Current State Analysis

### High Priority Candidates

#### 1. Floating Point Generators (`Arbi<float>`, `Arbi<double>`)

**Current API:**
```cpp
Arbi<float>(double nanProb = 0.0, double posInfProb = 0.0, double negInfProb = 0.0);
Arbi<double>(double nanProb = 0.0, double posInfProb = 0.0, double negInfProb = 0.0);
```

**Problem:** Three similar-typed parameters are easy to mix up:
```cpp
Arbi<float>(0.1, 0.05, 0.02);  // Which is which?
```

**Usage frequency:** Medium - used when testing floating point edge cases

#### 2. Container Generators (vector, list, set, string, etc.)

**Current API:**
```cpp
// Without element generator
Arbi<vector<T>>(size_t minSize = defaultMinSize, size_t maxSize = defaultMaxSize);

// With element generator
Arbi<vector<T>>(GenFunction<T> elemGen, size_t minSize = defaultMinSize, size_t maxSize = defaultMaxSize);
```

**Problem:** Parameter order can be confusing, especially when only setting some parameters:
```cpp
Arbi<vector<int>>(gen::int32(), 5, 20);  // Clear
Arbi<vector<int>>(5, 20);  // Clear
Arbi<vector<int>>(gen::int32(), 10);  // Is 10 minSize or maxSize?
```

**Usage frequency:** High - containers are very commonly used

#### 3. Optional Generator (`Arbi<optional<T>>`)

**Current API:**
```cpp
Arbi<optional<T>>(double nonEmptyProb = 0.95);
Arbi<optional<T>>(GenFunction<T> elemGen, double nonEmptyProb = 0.95);
```

**Problem:** When both parameters are used, the meaning of the double isn't immediately clear:
```cpp
Arbi<optional<int>>(gen::int32(), 0.9);  // What does 0.9 mean?
```

**Usage frequency:** Medium

#### 4. Map Generator (`Arbi<map<Key, Value>>`)

**Current API:**
```cpp
Arbi<map<Key, Value>>(size_t minSize = defaultMinSize, size_t maxSize = defaultMaxSize);
Arbi<map<Key, Value>>(GenFunction<pair<Key, Value>> pairGen, size_t minSize = defaultMinSize, size_t maxSize = defaultMaxSize);
```

**Problem:** Same as container generators - parameter order confusion

**Usage frequency:** Medium

#### 5. String Generators (string, UTF8String, etc.)

**Current API:**
```cpp
Arbi<string>(size_t minSize = defaultMinSize, size_t maxSize = defaultMaxSize);
Arbi<string>(Generator<char> elemGen, size_t minSize = defaultMinSize, size_t maxSize = defaultMaxSize);
```

**Problem:** Same as container generators

**Usage frequency:** High

### Lower Priority Candidates

- **Pair generator**: Only 2 parameters (both generators) - less confusing
- **Tuple generator**: Variadic parameters - named params less beneficial
- **Shared pointer generator**: Only 1 optional parameter - not needed

## Proposed API Design

### Approach 1: Config Structs (Recommended)

Create config structs similar to `ForAllConfig` for each generator type:

```cpp
// Floating point config
struct FloatGenConfig {
    optional<double> nanProb;
    optional<double> posInfProb;
    optional<double> negInfProb;
};

// Container config
struct ContainerGenConfig {
    optional<AnyGenerator> elemGen;  // Type-erased for template compatibility
    optional<size_t> minSize;
    optional<size_t> maxSize;
};

// Optional config
struct OptionalGenConfig {
    optional<AnyGenerator> elemGen;
    optional<double> nonEmptyProb;
};
```

**Pros:**
- Consistent with `ForAllConfig` pattern
- Type-safe
- Clear and self-documenting
- Easy to extend

**Cons:**
- Need type erasure for `elemGen` in config structs (or use templates)
- More boilerplate

### Approach 2: Factory Functions with Named Parameters

Create factory functions in `gen::` namespace that accept config structs:

```cpp
namespace gen {
    template <typename T>
    Arbi<T> vector(FloatGenConfig config = {}) { ... }
    
    template <typename T>
    Arbi<vector<T>> vector(ContainerGenConfig config = {}) { ... }
}
```

**Pros:**
- Cleaner syntax in `gen::` namespace
- Can provide better defaults
- More discoverable

**Cons:**
- Need to handle type inference
- May conflict with existing `gen::vector<T>` alias

### Approach 3: Hybrid Approach (Recommended)

Combine both: Add config struct constructors to `Arbi<>` classes AND provide factory functions in `gen::` namespace.

## Implementation Plan

### Phase 1: Floating Point Generators (Highest Priority)

**Why first:** Three similar-typed parameters are most confusing

**Implementation:**
1. Create `FloatGenConfig` struct
2. Add constructor to `Arbi<float>` and `Arbi<double>` accepting config
3. Keep existing positional constructors for backward compatibility
4. Add tests
5. Update documentation

**Example:**
```cpp
// Current
Arbi<float>(0.1, 0.05, 0.02);

// New
Arbi<float>({.nanProb = 0.1, .posInfProb = 0.05, .negInfProb = 0.02});

// Or using gen namespace factory
gen::float32({.nanProb = 0.1, .posInfProb = 0.05, .negInfProb = 0.02});
```

### Phase 2: Container Generators (High Priority)

**Why second:** Very commonly used, parameter order can be confusing

**Implementation:**
1. Create `ContainerGenConfig` struct (template or type-erased)
2. Add constructor to container generators (vector, list, set)
3. Keep existing positional constructors
4. Add tests
5. Update documentation

**Example:**
```cpp
// Current
Arbi<vector<int>>(gen::int32(), 5, 20);

// New
Arbi<vector<int>>({.elemGen = gen::int32(), .minSize = 5, .maxSize = 20});

// Partial config
Arbi<vector<int>>({.minSize = 5, .maxSize = 20});  // Uses default elemGen
Arbi<vector<int>>({.elemGen = gen::int32()});  // Uses default sizes
```

### Phase 3: String Generators

**Why third:** Similar to containers, commonly used

**Implementation:**
1. Reuse or extend container config pattern
2. Add to string generators (string, UTF8String, UTF16String, etc.)
3. Add tests
4. Update documentation

### Phase 4: Optional and Map Generators

**Why fourth:** Less commonly used, but still benefit from named params

**Implementation:**
1. Create `OptionalGenConfig` struct
2. Create `MapGenConfig` struct (similar to container)
3. Add constructors
4. Add tests
5. Update documentation

## Design Decisions

### Decision 1: Type Erasure for Element Generators

**Problem:** Config structs need to store generators, but generators are templated.

**Options:**
1. **Type-erased `AnyGenerator`**: Store as `optional<AnyGenerator>` in config
   - Pros: Simple, works for all types
   - Cons: Type information lost, need to cast back
   
2. **Template config structs**: `ContainerGenConfig<T>`
   - Pros: Type-safe
   - Cons: More complex, harder to use with designated initializers
   
3. **Separate configs**: One for with elemGen, one without
   - Pros: Type-safe, clear
   - Cons: More structs to maintain

**Recommendation:** Use type-erased `AnyGenerator` for simplicity and consistency. The generator is already type-erased internally anyway.

### Decision 2: Backward Compatibility

**Approach:** Keep all existing positional constructors. Add new config-based constructors as overloads.

**Rationale:** 
- Maintains backward compatibility
- Users can migrate gradually
- No breaking changes

### Decision 3: Factory Functions in `gen::` Namespace

**Question:** Should we add factory functions like `gen::vector<int>({...})`?

**Recommendation:** Yes, but as a Phase 2 enhancement. Start with direct `Arbi<>` constructors, then add factory functions if needed.

**Rationale:**
- Factory functions provide cleaner syntax
- Can be added later without breaking changes
- Start simple, iterate

### Decision 4: Partial Configuration

**Question:** Should all config fields be optional?

**Recommendation:** Yes, all fields optional. Use defaults for unspecified fields.

**Rationale:**
- Maximum flexibility
- Consistent with `ForAllConfig` pattern
- Users can specify only what they need to change

## Example Implementations

### Floating Point Generator Config

```cpp
struct FloatGenConfig {
    optional<double> nanProb;
    optional<double> posInfProb;
    optional<double> negInfProb;
};

// In Arbi<float>:
Arbi(const FloatGenConfig& config) 
    : nanProb(config.nanProb.value_or(0.0)),
      posInfProb(config.posInfProb.value_or(0.0)),
      negInfProb(config.negInfProb.value_or(0.0))
{
    // Validation...
}
```

### Container Generator Config

```cpp
struct ContainerGenConfig {
    optional<AnyGenerator> elemGen;
    optional<size_t> minSize;
    optional<size_t> maxSize;
};

// In Arbi<vector<T>>:
template <typename T>
Arbi(const ContainerGenConfig& config)
    : ArbiContainer<vector<T>>(
          config.minSize.value_or(defaultMinSize),
          config.maxSize.value_or(defaultMaxSize)
      ),
      elemGen(config.elemGen.has_value() 
          ? config.elemGen.value().template as<GenFunction<T>>()
          : Arbi<T>())
{
}
```

## Testing Plan

For each generator type:
1. **Basic config test**: All parameters specified
2. **Partial config test**: Only some parameters specified
3. **Empty config test**: Default config (should use defaults)
4. **Backward compatibility test**: Positional constructors still work
5. **Validation test**: Invalid configs are rejected (for floating point)

## Documentation Updates

1. **Generator documentation**: Add examples showing both old and new syntax
2. **Walkthrough**: Add examples of named parameter usage
3. **API reference**: Document config structs and new constructors

## Migration Path

1. **Phase 1**: Implement floating point generators
   - Users can start using immediately
   - Old code continues to work
   
2. **Phase 2-4**: Implement other generators incrementally
   - Each phase is independent
   - Can be adopted gradually

3. **Future**: Consider deprecating positional constructors (far future, if at all)

## Success Criteria

- ✅ All existing tests pass (backward compatibility)
- ✅ New tests for config-based constructors pass
- ✅ Documentation updated with examples
- ✅ Code is more readable and self-documenting
- ✅ No performance regression

## Risks and Mitigations

**Risk 1:** Type erasure complexity for element generators
- **Mitigation:** Use existing `AnyGenerator` infrastructure, test thoroughly

**Risk 2:** API surface bloat
- **Mitigation:** Keep config structs simple, only add where it significantly improves readability

**Risk 3:** Learning curve for new API
- **Mitigation:** Comprehensive documentation, examples, backward compatibility

## Timeline Estimate

- **Phase 1 (Floating Point)**: 1-2 days
- **Phase 2 (Containers)**: 2-3 days
- **Phase 3 (Strings)**: 1-2 days
- **Phase 4 (Optional/Map)**: 1-2 days
- **Total**: ~1-2 weeks for full implementation

## Next Steps

1. ✅ Add to TODO
2. Review and approve plan
3. Start with Phase 1 (Floating Point)
4. Implement, test, document
5. Iterate for remaining phases

---

**Status:** Planning Complete
**Last Updated:** 2025-01-20
**Author:** Planning document for generator named parameters enhancement
