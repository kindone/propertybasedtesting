# Shrink Tree Testing Summary Across All Projects

This document provides a comprehensive overview of how shrink tree testing is implemented across four property-based testing frameworks: cppproptest, jsproptest, python-proptest, and dartproptest.

## Overview

All four projects use **serialization tests** as the primary method for verifying shrink tree structure. However, each project has unique approaches, tools, and areas of focus that complement each other.

## Common Patterns (All Projects)

### 1. Serialization Tests (Primary Method)
All projects serialize shrink trees to verify exact structure:
- **Purpose**: Verify that shrink trees match expected structure deterministically
- **Usage**: Most common testing approach across all projects
- **Format**: Varies by project (see project-specific sections)

### 2. Structural Comparison
All projects provide `compareShrinkable()` functions:
- **Purpose**: Recursively compare two shrink trees for equality
- **Usage**: Less common than serialization, but available in all projects
- **Implementation**: Recursive traversal comparing values and child structures

### 3. Known Test Structures
All projects test with named structures:
- **40213**: Small structure (4 → [0, 2→1, 3])
- **7531246**: Medium structure (7 → [5→[3→1, 2], 4, 6])
- **964285173**: Large structure (9 → [6→[4→2, 8], 5→1, 7→3])
- **Purpose**: Provide deterministic test fixtures for combinator testing

## Project-Specific Approaches

### cppproptest

#### Testing Methods
1. **Serialization Tests** (Most Common)
   - Function: `serializeShrinkable()`
   - Format: Custom string format `{value: 8, shrinks: [{value: 0}, ...]}`
   - Example: `EXPECT_EQ(serializeShrinkable(shr), "{value: 8, shrinks: [{value: 0}, {value: 4, shrinks: [{value: 2, shrinks: [{value: 1}]}, {value: 3}]}, ...]}")`
   - Found in: `test_shrinker.cpp`, `test_combinator.cpp`, `test_combinator2.cpp`

2. **Domain/Predicate Tests**
   - Function: `checkShrinkTreeDomain()`
   - Purpose: Verify all values in tree satisfy a predicate (e.g., all floats are finite)
   - Example: `checkShrinkTreeDomain<float>(shrinkable, [](const float& val) { return isfinite(val); }, "finite")`
   - Found in: `test_floating.cpp`

3. **Exhaustive Printing** (Debugging)
   - Function: `printExhaustive()`
   - Purpose: Visualize entire trees for debugging
   - Found in: Various test files (often commented out)

#### Test Coverage
- **Basic types**: int, float, double, bool, string
- **Complex types**: pairs, tuples, vectors, lists
- **Combinators**: map, filter, flatMap, chain, oneOf, dependency, derive, accumulate, aggregate
- **Edge cases**: negative numbers, zero, large values, signed/unsigned
- **Constraint preservation**: Domain checks, filtering effects

#### Unique Strengths
- Strong domain/predicate testing with `checkShrinkTreeDomain()`
- Comprehensive coverage of all types and combinators
- Extensive edge case testing

---

### jsproptest

#### Testing Methods
1. **Serialization Tests** (Most Common)
   - Function: `serializeShrinkable()`
   - Format: JSON string `{"value":8,"shrinks":[...]}`
   - Example: `expect(serializeShrinkable(shr)).toEqual('{"value":4,"shrinks":[{"value":0},{"value":2,"shrinks":[{"value":1}]},{"value":3}]}')`
   - Found in: `shrinkable.test.ts` (primary), `generator.test.ts`

2. **Exhaustive Traversal Tests**
   - Function: `exhaustive()`
   - Purpose: Traverse entire trees with custom assertions
   - Example: `exhaustive(shrinkable, 0, assertInRangeAndUnique)`
   - Found in: `primitive.test.ts`, `generator.test.ts`

3. **Partial Structure Tests**
   - Purpose: Test specific parts of tree structure without full serialization
   - Example: Checking first-level children, specific tree paths
   - Found in: `generator.test.ts` (float shrinking)

#### Test Coverage
- **Shrinkable operations**: concat, andThen, map, filter, flatMap
- **Tree navigation**: getNthChild, retrieve (path-based access)
- **Known tree structures**: 40213, 7531246, 964285173 (named test cases)
- **Combinators**: Effects on shrink trees
- **Primitives**: Integer, float, set shrinking
- **Constraint preservation**: Range checks, uniqueness

#### Unique Strengths
- Strong focus on Shrinkable API operations (concat, andThen, etc.)
- Tree navigation testing (getNthChild, retrieve)
- Exhaustive traversal with custom assertions

---

### python-proptest

#### Testing Methods
1. **Serialization Tests** (Most Common)
   - Functions: `serialize_shrink_tree()`, `shrink_tree_to_string()`
   - Formats: JSON dict and compact string representation
   - Example: `self.assertEqual(tree_json["value"], expected_root)`
   - Found in: `test_tuple_shrink_tree.py`, `test_shrink_tree_expectations.py`, `test_shrinker_comprehensive.py`

2. **Expected Structure Tests** (Regression Tests)
   - Purpose: Test against known expected JSON representations
   - Example: Exact tree structure matching for specific seeds/values
   - Found in: `test_shrink_tree_expectations.py`, `test_tuple_shrink_tree.py`
   - Tests: Known values like integer 8, string "abc", tuple (10, 5, 5) with seed 42

3. **Constraint/Predicate Tests**
   - Function: `check_all_shrinks()`
   - Purpose: Verify all values in tree satisfy constraints
   - Example: All values in range, all satisfy filter predicate, constraint preservation
   - Found in: `test_flat_map_rng_preservation.py`, `test_flat_map_edge_cases.py`, `test_tuple_shrink_tree.py`

4. **Structural Comparison Tests**
   - Function: `compare_shrinkable()`
   - Purpose: Recursively compare tree structures
   - Found in: `test_shrinker_comprehensive.py`

5. **Consistency Tests**
   - Purpose: Test that serialization is deterministic (same tree on multiple calls)
   - Purpose: Test that trees from same seed are identical
   - Found in: Multiple test files

6. **Property-Based Tests**
   - Purpose: Test tree properties (uniqueness, recursive structure, all positions shrink)
   - Example: All values unique, recursive shrinking exists, all tuple positions can shrink
   - Found in: `test_tuple_shrink_tree.py`

#### Test Coverage
- **All primitive types**: int, float, bool, string
- **All container types**: list, tuple, dict, set
- **Combinators**: flat_map, chain, map, filter
- **Known structures**: 40213, 7531246, 964285173 (named test cases)
- **Deterministic seeds**: Tests with fixed seeds (e.g., 42) for reproducibility
- **Constraint preservation**: Range checks, type checks, filter predicates

#### Unique Strengths
- Extensive expected structure regression tests
- Property-based validation of tree characteristics
- Comprehensive constraint/predicate testing throughout trees
- Two serialization formats (JSON dict and compact string)
- Consistency and determinism testing

---

### dartproptest

#### Testing Methods
1. **Serialization Tests** (Most Common)
   - Function: `serializeShrinkable()`
   - Format: Compact JSON string `{"value":8,"shrinks":[...]}`
   - Example: `expect(serializeShrinkable(shr), equals('{"value":4,"shrinks":[{"value":0},{"value":2,"shrinks":[{"value":1}]},{"value":3}]}'))`
   - Found in: `shrinkable_comprehensive_test.dart`, `combinator_constraint_tree_test.dart`, `generator_test.dart`

2. **Expected Structure Tests** (Regression Tests)
   - Purpose: Test against known expected serialized representations
   - Uses named structures: 40213, 7531246, 964285173
   - Example: Exact tree structure matching for specific combinator operations
   - Found in: `combinator_constraint_tree_test.dart`, `shrinkable_comprehensive_test.dart`
   - Tests: map, filter, chain, flatMap, accumulate, aggregate, oneOf with known base structures

3. **Constraint/Predicate Tests**
   - Functions: `verifyConstraint()`, `collectAllValues()`
   - Purpose: Verify all values in tree satisfy constraints
   - Example: All values are even, all values are in range, constraint preservation
   - Found in: `combinator_constraint_tree_test.dart`

4. **Structural Comparison Tests**
   - Function: `compareShrinkable()`
   - Purpose: Recursively compare tree structures
   - Available in `testutil.dart` but less commonly used in tests

5. **Limited Serialization Tests** (Debugging)
   - Functions: `serializeLimited()`, `printTree()`
   - Purpose: Partial tree visualization
   - Found in: `show_float_tree_simple.dart`

6. **Uniqueness Tests**
   - Purpose: Test that all values in shrink tree are unique
   - Example: Using `Set` to track serialized strings and detect duplicates
   - Found in: `generator_test.dart`

#### Test Coverage
- **Known structures**: 40213, 7531246, 964285173 (named test cases, heavily used)
- **Combinators**: map, filter, chain, flatMap, oneOf, accumulate, aggregate
- **Shrinkable operations**: concat, andThen, map, filter, flatMap, retrieve
- **Constraint preservation**: Range checks, filter predicates, type constraints
- **All types**: int, float, bool, string, list, tuple, dict, set
- **Deterministic seeds**: Tests with fixed seeds for reproducibility

#### Unique Strengths
- Strong emphasis on combinator constraint preservation tests
- Uses named known structures (40213, 7531246, 964285173) as test fixtures
- Comprehensive constraint verification with `verifyConstraint()` and `collectAllValues()`
- Tests both exact structure and constraint preservation together
- Focus on combinator behavior (map, filter, chain, flatMap, accumulate, aggregate)

---

## Comparison Matrix

| Feature | cppproptest | jsproptest | python-proptest | dartproptest |
|---------|-------------|------------|-----------------|--------------|
| **Primary Method** | Serialization | Serialization | Serialization | Serialization |
| **Serialization Format** | Custom string | JSON string | JSON dict/string | Compact JSON |
| **Constraint Tests** | `checkShrinkTreeDomain()` | `exhaustive()` with assertions | `check_all_shrinks()` | `verifyConstraint()` |
| **Expected Structure Tests** | Some | Some | Extensive | Extensive |
| **Known Structures** | Yes | Yes | Yes | Yes (heavily used) |
| **Combinator Focus** | Medium | Low | Medium | **High** |
| **Property-Based** | Low | Low | **High** | Medium |
| **Uniqueness Tests** | No | No | Yes | Yes |
| **Consistency Tests** | No | No | Yes | No |
| **Tree Navigation** | No | **Yes** (getNthChild, retrieve) | No | Yes (retrieve) |
| **Shrinkable API Focus** | Low | **High** | Low | Medium |
| **Domain/Predicate Tests** | **High** | Medium | Medium | Medium |

## Common Test Coverage Areas

All projects test:
- ✅ **All primitive types**: int, float, bool, string
- ✅ **All container types**: list, tuple, dict, set, pair
- ✅ **Combinators**: map, filter, chain, flatMap, oneOf, accumulate, aggregate
- ✅ **Known structures**: 40213, 7531246, 964285173
- ✅ **Constraint preservation**: Range checks, type checks, filter predicates
- ✅ **Deterministic behavior**: Tests with fixed seeds for reproducibility

## Unique Strengths by Project

### cppproptest
- **Domain/predicate tests**: `checkShrinkTreeDomain()` for verifying all values satisfy constraints
- **Comprehensive coverage**: All types, combinators, edge cases
- **Custom serialization format**: Readable string representation

### jsproptest
- **Shrinkable API operations**: Extensive testing of concat, andThen, map, filter, flatMap
- **Tree navigation**: Tests for getNthChild, retrieve (path-based access)
- **Exhaustive traversal**: Custom assertions during tree traversal

### python-proptest
- **Expected structure regression tests**: Extensive tests against known JSON representations
- **Property-based validation**: Tests tree properties (uniqueness, recursive structure)
- **Comprehensive constraint testing**: `check_all_shrinks()` throughout trees
- **Dual serialization formats**: JSON dict and compact string
- **Consistency testing**: Deterministic serialization, RNG preservation

### dartproptest
- **Combinator constraint preservation**: Strong focus on preserving constraints through combinators
- **Named structure fixtures**: Uses 40213, 7531246, 964285173 as test fixtures
- **Combined approach**: Tests both exact structure and constraint preservation together
- **Combinator behavior**: Extensive testing of map, filter, chain, flatMap, accumulate, aggregate

## Testing Philosophy Comparison

### cppproptest
- **Approach**: Comprehensive coverage with domain verification
- **Focus**: Correctness of shrink trees across all types and combinators
- **Strength**: Domain/predicate testing ensures constraints are preserved

### jsproptest
- **Approach**: Shrinkable API-centric testing
- **Focus**: Correctness of Shrinkable operations and tree navigation
- **Strength**: API operation testing and tree traversal utilities

### python-proptest
- **Approach**: Regression testing with property-based validation
- **Focus**: Deterministic behavior and expected structure preservation
- **Strength**: Regression tests catch breaking changes, property tests validate tree characteristics

### dartproptest
- **Approach**: Combinator-focused constraint preservation
- **Focus**: Ensuring combinators preserve constraints while maintaining tree structure
- **Strength**: Combined structure and constraint verification

## Recommendations

### For New Implementations
1. **Start with serialization tests**: Most reliable and readable
2. **Add constraint verification**: Ensure all values in tree satisfy constraints
3. **Use known structures**: Create test fixtures (like 40213) for combinator testing
4. **Test determinism**: Verify same seed produces same tree
5. **Test uniqueness**: Ensure no duplicate values in shrink tree
6. **Test all types**: Cover primitives and containers
7. **Test combinators**: Verify structure preservation through transformations

### Best Practices
- Use deterministic seeds for regression tests
- Combine exact structure tests with constraint verification
- Test both small and large known structures
- Verify tree properties (uniqueness, recursive structure)
- Test constraint preservation through combinators
- Use serialization for readable test failures

## Conclusion

All four projects successfully use serialization as the primary testing method, with each project adding unique approaches:

- **cppproptest**: Domain/predicate verification
- **jsproptest**: Shrinkable API and tree navigation
- **python-proptest**: Regression tests and property-based validation
- **dartproptest**: Combinator constraint preservation

The approaches are complementary, and a comprehensive test suite would benefit from incorporating elements from all four projects.
