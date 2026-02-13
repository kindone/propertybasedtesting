# forAll Improvement Plan: Optional Configuration Parameters

## Overview

Add optional configuration parameters to `forAll()` using C++ designated initializers (C++20 feature). This will allow users to configure property tests directly in `forAll()` calls without needing to use the `property()` function for simple configuration cases.

## Current State

### Current API

**Current `forAll` signature:**
```cpp
template <typename Callable, typename... ExplicitGens>
bool forAll(Callable&& callable, ExplicitGens&&... gens)
```

**Current usage:**
```cpp
// Simple case - no configuration
forAll([](int x) { ... });

// With generators
forAll([](int x) { ... }, gen::int32());

// With configuration - requires property()
property([](int x) { ... })
    .setSeed(123)
    .setNumRuns(100)
    .setMaxDurationMs(5000)
    .setOnStartup([]() { ... })
    .setOnCleanup([]() { ... })
    .forAll();
```

### Available Configuration Options

From `Property` class:
1. `setSeed(uint64_t s)` - Set random seed for reproducibility
2. `setNumRuns(uint32_t runs)` - Set number of test runs
3. `setMaxDurationMs(uint32_t durationMs)` - Set maximum duration in milliseconds
4. `setOnStartup(Function<void()> _onStartup)` - Set startup callback
5. `setOnCleanup(Function<void()> _onCleanup)` - Set cleanup callback

## Proposed API

### New API Design

**New `forAll` overload with configuration:**
```cpp
// Configuration struct with designated initializers
struct ForAllConfig {
    optional<uint64_t> seed;
    optional<uint32_t> numRuns;
    optional<uint32_t> maxDurationMs;
    optional<Function<void()>> onStartup;
    optional<Function<void()>> onCleanup;
};

// New overload
template <typename Callable, typename... ExplicitGens>
bool forAll(Callable&& callable, ForAllConfig config, ExplicitGens&&... gens)
```

**New usage examples:**
```cpp
// Simple case - unchanged
forAll([](int x) { ... });

// With configuration using designated initializers
forAll([](int x) { ... }, {
    .seed = 123,
    .numRuns = 100
});

// With configuration and generators
forAll([](int x) { ... }, {
    .seed = 123,
    .numRuns = 100,
    .maxDurationMs = 5000
}, gen::int32());

// All configuration options
forAll([](int x) { ... }, {
    .seed = 123,
    .numRuns = 100,
    .maxDurationMs = 5000,
    .onStartup = []() { cout << "Starting test" << endl; },
    .onCleanup = []() { cout << "Cleaning up" << endl; }
});
```

## Implementation Plan

### Step 1: Define Configuration Struct

**Location:** `proptest/Property.hpp` (or new header `proptest/ForAllConfig.hpp`)

**Implementation:**
```cpp
namespace proptest {

struct ForAllConfig {
    optional<uint64_t> seed;
    optional<uint32_t> numRuns;
    optional<uint32_t> maxDurationMs;
    optional<Function<void()>> onStartup;
    optional<Function<void()>> onCleanup;

    // Default constructor - all options unset
    ForAllConfig() = default;
};

} // namespace proptest
```

**Considerations:**
- Use `optional<>` for all fields to allow partial configuration
- Default constructor initializes all fields as unset
- Designated initializers (C++20) allow named parameter syntax

### Step 2: Add Helper Function to Apply Configuration

**Location:** `proptest/Property.hpp` (as a private helper or in Property class)

**Implementation:**
```cpp
template <typename... ARGS>
void applyConfig(Property<ARGS...>& prop, const ForAllConfig& config)
{
    if (config.seed.has_value()) {
        prop.setSeed(config.seed.value());
    }
    if (config.numRuns.has_value()) {
        prop.setNumRuns(config.numRuns.value());
    }
    if (config.maxDurationMs.has_value()) {
        prop.setMaxDurationMs(config.maxDurationMs.value());
    }
    if (config.onStartup.has_value()) {
        prop.setOnStartup(config.onStartup.value());
    }
    if (config.onCleanup.has_value()) {
        prop.setOnCleanup(config.onCleanup.value());
    }
}
```

### Step 3: Add New `forAll` Overload

**Location:** `proptest/Property.hpp` (after existing `forAll` function)

**Implementation:**
```cpp
/**
 * @brief Immediately executes a randomized property test with configuration
 *
 * Allows configuration using designated initializers (C++20).
 * Equivalent to `property(...).setSeed(...).setNumRuns(...).forAll()`
 *
 * Usage:
 * @code
 * forAll([](int x) { ... }, {
 *     .seed = 123,
 *     .numRuns = 100
 * });
 * @endcode
 *
 * @tparam Callable property callable type
 * @tparam ExplicitGens Explicit generator callable types
 * @param callable property function
 * @param config configuration options (using designated initializers)
 * @param gens variadic list of generators (optional)
 * @return true if all the cases succeed
 * @return false if any one of the cases fails
 */
template <typename Callable, typename... ExplicitGens>
bool forAll(Callable&& callable, ForAllConfig config, ExplicitGens&&... gens)
{
    auto prop = property(util::forward<Callable>(callable), util::forward<ExplicitGens>(gens)...);
    applyConfig(prop, config);
    return prop.forAll();
}
```

**Considerations:**
- Must be placed after the existing `forAll` function
- Uses SFINAE or requires clause to ensure it doesn't conflict with existing overload
- Configuration is applied before calling `forAll()`

### Step 4: Handle Template Parameter Conflicts

**Problem:** The new overload might conflict with existing `forAll` when generators are passed.

**Solution Options:**

**Option A: Use SFINAE to distinguish**
```cpp
// Check if second parameter is ForAllConfig
template <typename Callable, typename Second, typename... Rest>
auto forAll(Callable&& callable, Second&& second, Rest&&... rest)
    -> enable_if_t<is_same_v<decay_t<Second>, ForAllConfig>, bool>
{
    // Configuration version
    auto prop = property(util::forward<Callable>(callable), util::forward<Rest>(rest)...);
    applyConfig(prop, second);
    return prop.forAll();
}

// Original version (when second param is not ForAllConfig)
template <typename Callable, typename... ExplicitGens>
auto forAll(Callable&& callable, ExplicitGens&&... gens)
    -> enable_if_t<!is_same_v<decay_t<tuple_element_t<0, tuple<ExplicitGens...>>>, ForAllConfig>, bool>
{
    return property(util::forward<Callable>(callable), util::forward<ExplicitGens>(gens)...).forAll();
}
```

**Option B: Use requires clause (C++20)**
```cpp
// Configuration version
template <typename Callable, typename Second, typename... Rest>
    requires is_same_v<decay_t<Second>, ForAllConfig>
bool forAll(Callable&& callable, Second&& config, Rest&&... gens)
{
    auto prop = property(util::forward<Callable>(callable), util::forward<Rest>(gens)...);
    applyConfig(prop, config);
    return prop.forAll();
}

// Original version
template <typename Callable, typename... ExplicitGens>
    requires (sizeof...(ExplicitGens) == 0 ||
              !is_same_v<decay_t<tuple_element_t<0, tuple<ExplicitGens...>>>, ForAllConfig>)
bool forAll(Callable&& callable, ExplicitGens&&... gens)
{
    return property(util::forward<Callable>(callable), util::forward<ExplicitGens>(gens)...).forAll();
}
```

**Recommendation:** Use Option B (requires clause) as it's cleaner and more modern C++20.

### Step 5: Update Documentation

**Files to update:**
1. `docs/PropertyAPI.md` - Add new `forAll` overload documentation
2. `docs/Walkthrough.md` - Add examples of new API
3. `docs/GettingStarted.md` - Update quick start examples if needed

**Documentation additions:**
- Explain designated initializer syntax
- Show examples of all configuration options
- Compare with `property()` API
- Note C++20 requirement for designated initializers

## Testing Plan

### Unit Tests

**Location:** `proptest/test/test_property.cpp`

**Test cases to add:**

1. **Basic configuration test:**
```cpp
TEST(Property, forAll_with_config_seed)
{
    forAll([](int x) {
        return true;
    }, {
        .seed = 42,
        .numRuns = 10
    });
}
```

2. **All configuration options:**
```cpp
TEST(Property, forAll_with_all_config)
{
    bool startupCalled = false;
    bool cleanupCalled = false;

    forAll([](int x) {
        return true;
    }, {
        .seed = 123,
        .numRuns = 5,
        .maxDurationMs = 1000,
        .onStartup = [&]() { startupCalled = true; },
        .onCleanup = [&]() { cleanupCalled = true; }
    });

    EXPECT_TRUE(startupCalled);
    EXPECT_TRUE(cleanupCalled);
}
```

3. **Configuration with generators:**
```cpp
TEST(Property, forAll_with_config_and_generators)
{
    forAll([](int x) {
        return x >= 0;
    }, {
        .seed = 0,
        .numRuns = 100
    }, gen::interval(0, 100));
}
```

4. **Partial configuration (only some options):**
```cpp
TEST(Property, forAll_with_partial_config)
{
    forAll([](int x) {
        return true;
    }, {
        .numRuns = 50
        // seed, maxDurationMs, etc. not set
    });
}
```

5. **Backward compatibility - existing calls still work:**
```cpp
TEST(Property, forAll_backward_compatible)
{
    // Should still work without config
    forAll([](int x) {
        return true;
    });

    // Should still work with generators
    forAll([](int x) {
        return true;
    }, gen::int32());
}
```

### Compile Tests

**Location:** `proptest/test/compile/` (if exists)

**Test cases:**
- Verify designated initializer syntax compiles (C++20)
- Verify SFINAE/requires correctly distinguishes overloads
- Verify template parameter deduction works correctly

## Backward Compatibility

### Requirements

1. **Existing code must continue to work:**
   - `forAll(callable)` - no changes
   - `forAll(callable, gen1, gen2, ...)` - no changes
   - `property(...).forAll()` - no changes

2. **No breaking changes:**
   - All existing tests should pass
   - All existing examples should work

### Verification

- Run all existing tests
- Check that no existing code breaks
- Verify template parameter deduction works correctly

## C++ Standard Requirements

### C++20 Features Used

1. **Designated Initializers** (C++20):
   - Required for the syntax: `{.seed = 123, .numRuns = 100}`
   - Not available in C++17

2. **Requires Clause** (C++20):
   - Used for template constraints
   - Can use SFINAE as fallback for C++17 compatibility (if needed)

### Compatibility Considerations

- **C++20 required** for designated initializers
- Check if cppproptest2 already requires C++20
- If C++17 support is needed, consider alternative API design

## Alternative Designs (if C++17 support needed)

### Option 1: Builder Pattern
```cpp
forAll([](int x) { ... })
    .withSeed(123)
    .withNumRuns(100)
    .run();
```

### Option 2: Named Parameters (using struct)
```cpp
ForAllConfig config;
config.seed = 123;
config.numRuns = 100;
forAll([](int x) { ... }, config);
```

### Option 3: Variadic Template with Tag Types
```cpp
forAll([](int x) { ... },
    seed(123),
    numRuns(100)
);
```

**Recommendation:** Use designated initializers if C++20 is acceptable, as it provides the cleanest API.

## Implementation Checklist

- [ ] **Step 1:** Define `ForAllConfig` struct
  - [ ] Add to appropriate header file
  - [ ] Use `optional<>` for all fields
  - [ ] Add default constructor

- [ ] **Step 2:** Implement `applyConfig` helper function
  - [ ] Handle all configuration options
  - [ ] Check for optional values before applying

- [ ] **Step 3:** Add new `forAll` overload
  - [ ] Use requires clause or SFINAE for overload resolution
  - [ ] Apply configuration before calling `forAll()`
  - [ ] Support generators as additional parameters

- [ ] **Step 4:** Handle template parameter conflicts
  - [ ] Ensure correct overload selection
  - [ ] Test with various parameter combinations

- [ ] **Step 5:** Add comprehensive tests
  - [ ] Basic configuration
  - [ ] All configuration options
  - [ ] Configuration with generators
  - [ ] Partial configuration
  - [ ] Backward compatibility

- [ ] **Step 6:** Update documentation
  - [ ] PropertyAPI.md - API reference
  - [ ] Walkthrough.md - Usage examples
  - [ ] GettingStarted.md - Quick examples

- [ ] **Step 7:** Verify backward compatibility
  - [ ] Run all existing tests
  - [ ] Check existing examples still work
  - [ ] No breaking changes

## Benefits

1. **Cleaner API:** Direct configuration in `forAll()` call
2. **Less boilerplate:** No need for `property()` for simple cases
3. **Readable:** Designated initializers make configuration clear
4. **Flexible:** All configuration options available
5. **Backward compatible:** Existing code continues to work

## Potential Issues and Solutions

### Issue 1: Template Parameter Ambiguity

**Problem:** When generators are passed, compiler might not know if second parameter is config or generator.

**Solution:** Use requires clause or SFINAE to distinguish based on type.

### Issue 2: C++20 Requirement

**Problem:** Designated initializers require C++20.

**Solution:**
- Check if cppproptest2 already requires C++20
- If not, document C++20 requirement for this feature
- Or provide alternative API for C++17

### Issue 3: Optional Type Support

**Problem:** Need `optional<>` type support.

**Solution:**
- Check if cppproptest2 already has `optional` (likely `std::optional` or custom)
- Use existing optional implementation

## Example Usage Comparison

### Before (current API):
```cpp
// Simple case
forAll([](int x) { ... });

// With configuration
property([](int x) { ... })
    .setSeed(123)
    .setNumRuns(100)
    .forAll();

// With configuration and generators
property([](int x) { ... }, gen::int32())
    .setSeed(123)
    .setNumRuns(100)
    .forAll();
```

### After (proposed API):
```cpp
// Simple case - unchanged
forAll([](int x) { ... });

// With configuration - new, cleaner syntax
forAll([](int x) { ... }, {
    .seed = 123,
    .numRuns = 100
});

// With configuration and generators - new, cleaner syntax
forAll([](int x) { ... }, {
    .seed = 123,
    .numRuns = 100
}, gen::int32());

// Old property() API still works for complex cases
property([](int x) { ... })
    .setSeed(123)
    .setNumRuns(100)
    .example(42)  // Can still use other methods
    .forAll();
```

## Next Steps

1. Review and approve this plan
2. Check C++ standard requirements (C++20 support)
3. Check existing `optional` implementation
4. Implement Step 1 (ForAllConfig struct)
5. Implement Step 2 (applyConfig helper)
6. Implement Step 3 (new forAll overload)
7. Add tests (Step 5)
8. Update documentation (Step 6)
9. Verify backward compatibility (Step 7)

---

**Status:** Planning Complete
**Last Updated:** 2025-01-20
**Author:** Planning document for cppproptest2 forAll improvement
