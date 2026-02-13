# TODO: cppproptest2 Development Tasks

This file tracks development tasks, improvements, and feature enhancements for the C++ property-based testing framework (`cppproptest2`).

## Completed

### [x] `forAll` Improvement - Optional Configuration Parameters
- **Status**: ✅ Completed
- **Description**: Added optional configuration parameters to `forAll()` using C++ designated initializers (C++20). Also added `Property::setConfig()` for batch configuration.
- **Plan**: See [archive/forAll_improvement_plan.md](archive/forAll_improvement_plan.md) for detailed implementation plan (archived - task completed)
- **Key Features Implemented**:
  - ✅ `ForAllConfig` struct with designated initializers
  - ✅ Support for all configuration options (seed, numRuns, maxDurationMs, onStartup, onCleanup)
  - ✅ New `forAll()` overload accepting `ForAllConfig`
  - ✅ `Property::setConfig()` method for batch configuration
  - ✅ Backward compatible with existing API
  - ✅ Works with generators
  - ✅ Comprehensive tests (10 new test cases)
  - ✅ Documentation updated (PropertyAPI.md, Walkthrough.md)
- **Implementation Date**: 2025-01-20
- **Files Modified**:
  - `proptest/Property.hpp` - Added `ForAllConfig`, `setConfig()`, and new `forAll()` overload
  - `proptest/test/test_property.cpp` - Added 10 test cases
  - `docs/PropertyAPI.md` - Updated API documentation
  - `docs/Walkthrough.md` - Added examples and usage guidance

## In Progress

### [ ] Generator Constructors with Named Parameters (C++20)
- **Status**: Planning Complete
- **Description**: Add named parameter support to generator constructors using C++20 designated initializers, similar to `ForAllConfig`. This will make generator APIs more readable and self-documenting.
- **Plan**: See [generator_named_params_plan.md](generator_named_params_plan.md) for detailed implementation plan
- **Motivation**: Many generators take multiple parameters that can be confusing when passed positionally (e.g., `Arbi<float>(0.1, 0.05, 0.02)` - which probability is which?)
- **Key Candidates** (Priority Order):
  1. **Floating point generators** (`Arbi<float>`, `Arbi<double>`) - 3 similar probability parameters
  2. **Container generators** (vector, list, set) - element generator + size bounds
  3. **String generators** (string, UTF8String, etc.) - similar to containers
  4. **Optional generator** - element generator + probability
  5. **Map generator** - pair generator + size bounds
- **Proposed API Examples**:
  ```cpp
  // Floating point - current vs proposed
  Arbi<float>(0.1, 0.05, 0.02);  // Current - unclear which is which
  Arbi<float>({.nanProb = 0.1, .posInfProb = 0.05, .negInfProb = 0.02});  // Proposed

  // Container - current vs proposed
  Arbi<vector<int>>(gen::int32(), 5, 20);  // Current
  Arbi<vector<int>>({.elemGen = gen::int32(), .minSize = 5, .maxSize = 20});  // Proposed
  ```
- **Implementation Phases**:
  - Phase 1: Floating point generators (highest priority - 3 similar params)
  - Phase 2: Container generators (high usage, parameter confusion)
  - Phase 3: String generators (similar to containers)
  - Phase 4: Optional and Map generators
- **Design Decisions**:
  - Use config structs similar to `ForAllConfig`
  - All fields optional (use defaults for unspecified)
  - Maintain backward compatibility with positional constructors
  - Type-erased `AnyGenerator` for element generators in configs
- **C++ Standard**: Requires C++20 for designated initializers
- **Next Steps**:
  1. Review and approve plan
  2. Start Phase 1: Implement floating point generator configs
  3. Add tests and documentation
  4. Iterate for remaining phases

## Future Enhancements

### [ ] Regex-Based String Generator
- **Status**: Planning
- **Description**: Add a string generator that can generate strings matching a regular expression pattern. This is a powerful feature that allows generating test data conforming to specific formats (e.g., email addresses, phone numbers, URLs, custom formats).
- **Motivation**: 
  - Many tests need strings matching specific patterns (email, phone, UUID, etc.)
  - Currently requires custom generators or filtering
  - Common feature in other property-based testing frameworks (QuickCheck, Hypothesis, etc.)
  - Would significantly enhance string generation capabilities
- **Use Cases**:
  - Email addresses: `gen::regex("[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}")`
  - Phone numbers: `gen::regex("\\+?[1-9]\\d{1,14}")`
  - UUIDs: `gen::regex("[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}")`
  - Custom formats: `gen::regex("ABC-\\d{4}-[A-Z]{2}")`
  - IP addresses: `gen::regex("(?:[0-9]{1,3}\\.){3}[0-9]{1,3}")`
- **Proposed API**:
  ```cpp
  // Basic usage
  auto emailGen = gen::regex("[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}");
  
  // With size constraints
  auto phoneGen = gen::regex("\\+?[1-9]\\d{1,14}", {.minLength = 10, .maxLength = 15});
  
  // In property tests
  forAll([](const string& email) {
      // Test email validation
  }, gen::regex("[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}"));
  ```
- **Technical Considerations**:
  - **Regex Engine**: Need to choose/implement regex engine
    - Option 1: Use `std::regex` (C++11 standard library)
      - Pros: Standard, no dependencies
      - Cons: Limited features, performance concerns, different implementations across compilers
    - Option 2: Use external library (e.g., RE2, PCRE2)
      - Pros: Better performance, more features, consistent behavior
      - Cons: External dependency, build complexity
    - Option 3: Custom lightweight regex engine
      - Pros: Full control, optimized for generation
      - Cons: Significant implementation effort
  - **Generation Strategy**:
    - Parse regex into AST
    - Generate strings by traversing AST
    - Ensure generated strings match pattern
    - Handle character classes, quantifiers, alternation, etc.
  - **Shrinking**: Need to implement shrinking for regex-generated strings
    - Shrink towards simpler/shorter strings that still match pattern
    - May need regex-aware shrinking strategy
  - **Performance**: Regex generation can be expensive
    - Consider caching parsed regex
    - Optimize common patterns
    - May need size limits to prevent infinite generation
- **Implementation Challenges**:
  1. **Full regex support**: Supporting all regex features (lookahead, backreferences, etc.) is complex
  2. **Generation completeness**: Ensuring all valid strings can be generated (not just some subset)
  3. **Shrinking**: Shrinking regex-matched strings while maintaining pattern match
  4. **Unicode support**: Handling Unicode in regex patterns
  5. **Performance**: Efficient generation for complex patterns
- **Phased Approach**:
  - **Phase 1**: Basic regex support (character classes, quantifiers, alternation)
  - **Phase 2**: More advanced features (groups, anchors)
  - **Phase 3**: Optimization and performance improvements
  - **Phase 4**: Advanced features (lookahead, backreferences) if needed
- **Research Needed**:
  - Study how other PBT frameworks implement regex generation (Hypothesis, QuickCheck, etc.)
  - Evaluate regex engine options
  - Design generation algorithm
  - Design shrinking strategy
- **Dependencies**:
  - C++ standard library (`std::regex`) or external regex library
  - May need Unicode support library
- **Next Steps**:
  1. Research existing implementations in other PBT frameworks
  2. Evaluate regex engine options
  3. Design generation algorithm
  4. Create detailed implementation plan
  5. Prototype basic implementation
  6. Add tests and documentation

### [ ] `gen::oneOf` Value Arguments (Implicit `just`-Wrapping)
- **Status**: Planning
- **Description**: Enhance `gen::oneOf<T>` so it can take raw values of `T` (in addition to generators of `T`) and implicitly wrap them as `gen::just<T>`. This would align `oneOf` with `elementOf`, making APIs more ergonomic and expressive.
- **Current API**:
  ```cpp
  // Requires generators
  auto gen = gen::oneOf<int>(
      gen::just<int>(1339),
      gen::just<int>(42)
  );
  ```
- **Proposed API**:
  ```cpp
  // Allow raw values (implicitly treated as gen::just<T>(value))
  auto gen = gen::oneOf<int>(1339, 42);

  // Mix of generators, weighted generators, and raw values
  auto gen2 = gen::oneOf<int>(
      0,
      gen::int32(),
      gen::weightedGen<int>(gen::int32(), 0.1),
      100
  );
  ```
- **Motivation**:
  - Reduce boilerplate when combining a few fixed values with generators
  - Make `oneOf` symmetric with `elementOf`, which already accepts raw values and weights
  - Improve readability and discoverability of simple “one-of-these-values-or-random” patterns
- **Design Considerations**:
  - `gen::oneOf<T>` currently requires each argument to be either:
    - A generator `GEN` such that `GEN` is `GenLike<GEN, T>`, or
    - A weighted wrapper `util::Weighted<T>` (from `weightedGen`)
  - Need a mechanism similar to `ValueToWeighted<T>` used by `elementOf` to:
    - Accept raw values `T` (and possibly `WeightedValue<T>`)
    - Convert them into generators (via `gen::just<T>`) and then into `WeightedBase`
  - Must maintain backward compatibility with existing `oneOf` usages
- **Possible Approaches**:
  1. **Overload `gen::oneOf`**:
     - Add a new overload that takes a parameter pack of `Impl...` where each `Impl` can be:
       - A generator of `T`
       - A weighted generator (`util::Weighted<T>`)
       - A raw value `T`
     - Internally map each argument to a `WeightedBase`:
       - Generators: via existing `GenToWeighted<T>`
       - Raw values: via `gen::just<T>(value)` + weight 0 (filled in evenly later)
  2. **Extend constraints of existing template**:
     - Relax the `requires` clause so that it also accepts raw `T` and route them through a helper similar to `ValueToWeighted<T>`
  3. **Introduce helper wrapper**:
     - Allow `oneOf<T>(value(v1), value(v2), gen1, gen2)` where `value(...)` marks raw values
- **Related Existing Functionality**:
  - `gen::elementOf<T>` already:
    - Accepts raw values (and optional explicit weights)
    - Converts them into `WeightedValue<T>` and then to `WeightedBase` using `gen::just<T>`
  - `util::oneOfImpl` already operates on a vector of `WeightedBase`, independent of how those were created
- **Technical Considerations**:
  - Template constraint complexity: need to ensure overload resolution remains unambiguous
  - Avoid accidental acceptance of wrong types (e.g., mixing `T` and unrelated types)
  - Preserve performance by doing wrapping at construction time, not per-sample
- **Potential Extensions**:
  - Add similar implicit `just`-wrapping to other combinators where it makes sense:
    - `gen::oneOf` for complex types (e.g., enums, tagged unions)
    - Possibly `gen::dependency` / higher-level combinators where raw values are common
- **Next Steps**:
  1. Audit all `oneOf` usages in cppproptest2 to understand patterns
  2. Sketch overload design and constraints that safely accept raw `T`
  3. Prototype implementation and add focused tests:
     - Raw values only
     - Mix of generators and raw values
     - Mix of weighted generators and raw values
  4. Compare ergonomics and maintain backward compatibility
  5. Update `Combinators.md` and `GenNamespace.md` with examples