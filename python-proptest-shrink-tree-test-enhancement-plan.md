# Shrink Tree Test Enhancement Plan for python-proptest

Based on comparison with cppproptest, jsproptest, and dartproptest, here are recommended test additions for python-proptest.

## Current State

### What python-proptest has:
- ✅ Serialization tests (`serialize_shrink_tree`, `shrink_tree_to_string`)
- ✅ Known structure 40213
- ✅ Basic combinator tests (map, filter, flatMap)
- ✅ Constraint preservation tests (general)
- ✅ Comprehensive shrinkable operation tests (concat, andThen, map, filter, flatMap)
- ✅ Expected structure regression tests

### What's missing compared to other projects:

## 1. Additional Known Structures (from dartproptest)

**Missing:** 7531246 and 964285173 structures

**Location:** `tests_integration/shrinker/test_shrinker_comprehensive.py`

**Add:**
```python
def gen_shrinkable_7531246() -> Shrinkable[int]:
    """Generate a shrinkable with value 7 and shrinks [5->[3->1, 2], 4, 6]."""
    return Shrinkable(7).with_shrinks(
        lambda: Stream.many([
            Shrinkable(5).with_shrinks(lambda: Stream.many([
                Shrinkable(3).with_shrinks(lambda: Stream.one(Shrinkable(1))),
                Shrinkable(2),
            ])),
            Shrinkable(4),
            Shrinkable(6),
        ])
    )

def gen_shrinkable_964285173() -> Shrinkable[int]:
    """Generate a shrinkable with value 9 and shrinks [6->[4->2, 8], 5->1, 7->3]."""
    return Shrinkable(9).with_shrinks(
        lambda: Stream.many([
            Shrinkable(6).with_shrinks(lambda: Stream.many([
                Shrinkable(4).with_shrinks(lambda: Stream.one(Shrinkable(2))),
                Shrinkable(8),
            ])),
            Shrinkable(5).with_shrinks(lambda: Stream.one(Shrinkable(1))),
            Shrinkable(7).with_shrinks(lambda: Stream.one(Shrinkable(3))),
        ])
    )
```

**Tests to add:**
- `test_shrinkable_7531246()` - Verify serialization matches expected structure
- `test_shrinkable_964285173()` - Verify serialization matches expected structure
- Tests using these structures with combinators (map, filter, chain, etc.)

## 2. Domain/Predicate Tests (from cppproptest)

**Missing:** `checkShrinkTreeDomain()` function and tests

**Location:** New file `tests_integration/shrinker/test_shrink_tree_domain.py`

**Add:**
```python
def check_shrink_tree_domain(shrinkable, predicate, domain_name="domain", max_depth=100):
    """
    Verify all values in a shrink tree satisfy a predicate.
    
    Similar to cppproptest's checkShrinkTreeDomain.
    
    Args:
        shrinkable: The Shrinkable to check
        predicate: Function that returns True if value satisfies domain
        domain_name: Name of domain for error messages
        max_depth: Maximum depth to traverse
    """
    def check_recursive(sh, depth=0):
        if depth > max_depth:
            return []
        problems = []
        if not predicate(sh.value):
            problems.append((depth, sh.value, f"violates {domain_name}"))
        for child in sh.shrinks().to_list():
            problems.extend(check_recursive(child, depth + 1))
        return problems
    
    problems = check_recursive(shrinkable)
    if problems:
        raise AssertionError(
            f"Found {len(problems)} values violating {domain_name}: {problems[:10]}"
        )
```

**Tests to add:**
- `test_float_finite_only_shrinks_preserve_domain()` - All floats in tree are finite
- `test_float_with_nan_probability_domain()` - NaN values are valid
- `test_int_range_domain()` - All integers in tree are within range
- `test_string_length_domain()` - All strings in tree respect length constraints
- `test_list_size_domain()` - All lists in tree respect size constraints

## 3. Combinator Constraint Preservation with Known Structures (from dartproptest)

**Missing:** Comprehensive combinator tests using known structures (40213, 7531246, 964285173)

**Location:** New file `tests_integration/combinator/test_combinator_shrink_trees.py`

**Add tests for each combinator using known structures:**

### Map Tests
- `test_map_preserves_tree_structure_40213()` - Map x*2 on 40213 structure
- `test_map_preserves_tree_structure_7531246()` - Map on larger structure
- `test_map_preserves_tree_structure_964285173()` - Map on largest structure
- Verify both structure AND constraint preservation

### Filter Tests
- `test_filter_preserves_tree_structure_40213()` - Filter x>=2 on 40213
- `test_filter_preserves_tree_structure_7531246()` - Filter on larger structure
- Verify filtered tree structure matches expected
- Verify all values in filtered tree satisfy predicate

### Chain Tests
- `test_chain_preserves_tree_structure_40213()` - Chain with dependent values
- Verify tuple structure is correct
- Verify constraint preservation (e.g., second <= first)

### FlatMap Tests
- `test_flat_map_preserves_tree_structure_40213()` - FlatMap with nested dependency
- Verify list constraints are preserved
- Verify all elements satisfy constraints

### Aggregate Tests
- `test_aggregate_preserves_tree_structure_40213()` - Aggregate with variable length
- Test length-based shrinking
- Test element-based shrinking
- Verify length and element constraints

### Accumulate Tests
- `test_accumulate_preserves_tree_structure_40213()` - Accumulate with variable length
- Test final value only (not intermediate)
- Verify constraints on final value

### OneOf Tests
- `test_one_of_preserves_tree_structure()` - OneOf with known structures
- Verify selected generator's tree structure is preserved

## 4. Helper Functions (from dartproptest)

**Missing:** `collectAllValues()` and `verifyConstraint()` helpers

**Location:** Add to `tests_integration/shrinker/test_shrinker_comprehensive.py` or create `testutil.py`

**Add:**
```python
def collect_all_values(shrinkable, all_values=None, max_depth=100):
    """Collect all values from a shrinkable tree into a set."""
    if all_values is None:
        all_values = set()
    if max_depth <= 0:
        return all_values
    all_values.add(shrinkable.value)
    for child in shrinkable.shrinks().to_list():
        collect_all_values(child, all_values, max_depth - 1)
    return all_values

def verify_constraint(shrinkable, constraint, max_depth=100):
    """Verify that all values in a shrinkable tree satisfy a constraint."""
    if max_depth <= 0:
        return True
    if not constraint(shrinkable.value):
        return False
    for child in shrinkable.shrinks().to_list():
        if not verify_constraint(child, constraint, max_depth - 1):
            return False
    return True
```

## 5. Float Domain Tests (from cppproptest)

**Missing:** Specific tests for float domain preservation

**Location:** New file `tests_integration/generator/test_float_shrink_tree_domain.py`

**Add:**
- `test_float_finite_only_shrinks_preserve_domain()` - Default float gen (finite only)
- `test_float_with_nan_probability_domain()` - Float gen with NaN probability
- `test_float_with_posinf_probability_domain()` - Float gen with +inf probability
- `test_float_with_neginf_probability_domain()` - Float gen with -inf probability
- `test_float_with_all_probabilities_domain()` - Float gen with all special values

## 6. Tree Navigation Tests (from jsproptest)

**Missing:** Tests for tree navigation operations (if python-proptest has them)

**Note:** This depends on whether python-proptest exposes tree navigation APIs. If not, skip this.

## Implementation Priority

### High Priority (Core Functionality)
1. ✅ Add 7531246 and 964285173 structures
2. ✅ Add `check_shrink_tree_domain()` helper
3. ✅ Add combinator tests with known structures (map, filter, chain, flatMap)
4. ✅ Add float domain tests

### Medium Priority (Enhanced Coverage)
5. ✅ Add aggregate/accumulate tests with known structures
6. ✅ Add oneOf tests with known structures
7. ✅ Add `collect_all_values()` and `verify_constraint()` helpers

### Low Priority (Nice to Have)
8. Tree navigation tests (if API exists)
9. More edge case combinations

## Files to Create/Modify

1. **Modify:** `tests_integration/shrinker/test_shrinker_comprehensive.py`
   - Add `gen_shrinkable_7531246()` and `gen_shrinkable_964285173()`
   - Add tests for these structures

2. **Create:** `tests_integration/shrinker/test_shrink_tree_domain.py`
   - Add `check_shrink_tree_domain()` helper
   - Add domain tests for all generator types

3. **Create:** `tests_integration/combinator/test_combinator_shrink_trees.py`
   - Add comprehensive combinator tests using known structures
   - Test both structure preservation and constraint preservation

4. **Create:** `tests_integration/generator/test_float_shrink_tree_domain.py`
   - Add float-specific domain tests

5. **Modify:** `tests_integration/shrinker/test_shrinker_comprehensive.py` or create `testutil.py`
   - Add `collect_all_values()` and `verify_constraint()` helpers

## Implementation Status

### ✅ Completed
- Added 7531246 and 964285173 structures to `test_shrinker_comprehensive.py`
- Added tests for 7531246 and 964285173 structures
- Created `test_shrink_tree_domain.py` with `check_shrink_tree_domain()` helper
- Created `test_combinator_shrink_trees.py` with map, filter, chain, flatMap tests
- Added `collect_all_values()` and `verify_constraint()` helpers to combinator tests
- Added comprehensive float, int, string, and list domain tests

### 🔄 In Progress / Future
- More comprehensive chain tests with exact structure verification
- Aggregate/accumulate tests with known structures (like dartproptest)
- OneOf tests with known structures
- More complex filter scenarios
- Tests combining multiple combinators

