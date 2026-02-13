# Constraint Preservation Test Coverage Summary

This document summarizes the current test coverage for ensuring generators and combinators properly generate shrinks that honor their constraints.

## Test Files

### Primary Test Files
1. **`test_shrink_tree_domain.py`** - Tests domain/predicate verification for primitive and collection generators
2. **`test_constraint_preservation.py`** - Tests constraint preservation across generators with transformations
3. **`test_combinator_shrink_trees.py`** - Tests combinator shrink tree structure preservation

## Current Coverage

### ✅ Fully Tested Generators

#### Primitive Generators
- **`Gen.int`** ✅
  - Range constraints (`min_value`, `max_value`) - `test_int_range_domain`
  - Negative range constraints - `test_int_negative_range_domain`
  - Constraint preservation with map - `test_int_map_*` (additive, multiplicative, complex)
  - Constraint preservation with filter - `test_int_filter_preserves_constraints`
  - Boundary cases (min/max) - `test_int_at_*_boundary`

- **`Gen.float`** ✅
  - Finite-only domain - `test_float_finite_only_shrinks_preserve_domain`
  - NaN probability - `test_float_with_nan_probability_domain`
  - +inf probability - `test_float_with_posinf_probability_domain`
  - -inf probability - `test_float_with_neginf_probability_domain`
  - All special values - `test_float_with_all_probabilities_domain`
  - Note: Float shrinking can go outside range (expected behavior)

- **`Gen.str`** ✅
  - Length constraints (`min_length`, `max_length`) - `test_string_length_domain`
  - Constraint preservation with map - `test_str_length_constraints_preserved`

#### Collection Generators
- **`Gen.list`** ✅
  - Size constraints (`min_length`, `max_length`) - `test_list_size_domain`
  - Element constraints - `test_list_element_domain`
  - Uniqueness when root has unique values - `test_list_shrink_uniqueness_with_unique_root`
  - Constraint preservation with constrained elements - `test_list_with_constrained_elements_preserves_constraints`
  - Constraint transformation with map - `test_list_map_transforms_element_constraints`

### ⚠️ Partially Tested Generators

- **`Gen.dict`** ⚠️
  - Tested in nested structures (`test_list_of_dicts_preserves_constraints`, `test_dict_of_lists_preserves_constraints`)
  - Missing: Direct dict constraint preservation test (size constraints, key/value constraints)

- **`Gen.set`** ⚠️
  - Tested in nested structures (`test_set_with_constrained_elements_preserves_constraints`)
  - Missing: Direct set constraint preservation test (size constraints)

- **`Gen.tuple`** ⚠️
  - Tested in `test_tuple_shrink_tree.py` for structure
  - Missing: Constraint preservation test for tuple element constraints

### ❌ Not Tested Generators

- **`Gen.bool`** ❌
  - No constraint preservation tests

- **`Gen.one_of`** / **`Gen.union_of`** ❌
  - No constraint preservation tests

- **`Gen.just`** ❌
  - No constraint preservation tests (though trivial - always same value)

- **`Gen.element_of`** ❌
  - No constraint preservation tests

- **`Gen.weighted_gen`** / **`Gen.weighted_value`** ❌
  - No constraint preservation tests

- **`Gen.unique_list`** ❌
  - No constraint preservation tests

- **`Gen.natural`** ❌
  - No constraint preservation tests

- **`Gen.non_negative`** ❌
  - No constraint preservation tests

- **`Gen.aggregate`** ❌
  - No constraint preservation tests

- **`Gen.accumulate`** ❌
  - No constraint preservation tests

- **`Gen.chain`** / **`Gen.chain_tuple`** ❌
  - No constraint preservation tests

### ✅ Fully Tested Combinators

- **`map`** ✅
  - Int transformations (additive, multiplicative, complex) - `test_int_map_*`
  - List element transformations - `test_list_map_transforms_element_constraints`
  - String length constraints - `test_str_length_constraints_preserved`
  - Map+filter chain - `test_map_filter_chain_preserves_all_constraints`

- **`filter`** ✅
  - Int constraints with filter - `test_int_filter_preserves_constraints`
  - Map+filter chain - `test_map_filter_chain_preserves_all_constraints`

### ⚠️ Partially Tested Combinators

- **`flat_map`** ⚠️
  - Tested in `test_flat_map_rng_preservation.py` for RNG preservation
  - Missing: Constraint preservation tests

- **`chain`** ⚠️
  - Tested in `test_chain_*.py` for basic functionality
  - Missing: Constraint preservation tests

## Test Patterns

### Existing Test Pattern
```python
def test_generator_constraints_preserved(self):
    rng = random.Random(42)
    gen = Gen.generator(...)  # with constraints
    
    for _ in range(N):
        shrinkable = gen.generate(rng)
        # Check root respects constraints
        self.assertConstraint(shrinkable.value)
        
        # Check all shrinks respect constraints
        check_shrink_tree_domain(
            shrinkable,
            lambda x: constraint_check(x),
            "constraint name"
        )
```

## Missing Coverage Summary

### Generators Needing Tests
1. **`Gen.dict`** - Direct size and key/value constraint tests
2. **`Gen.set`** - Direct size constraint tests
3. **`Gen.tuple`** - Element constraint preservation
4. **`Gen.bool`** - Constraint preservation (if any)
5. **`Gen.one_of`** - Constraint preservation for selected generator
6. **`Gen.unique_list`** - Size and uniqueness constraints
7. **`Gen.aggregate`** - Size and element constraints
8. **`Gen.accumulate`** - Size and dependency constraints
9. **`Gen.chain`** - Constraint preservation through chain
10. **`Gen.natural`** / **`Gen.non_negative`** - Range constraints

### Combinators Needing Tests
1. **`flat_map`** - Constraint preservation through flat_map
2. **`chain`** - Constraint preservation through chain

## Notes

- **Size constraints vs Element constraints**: Tests correctly distinguish between:
  - Size constraints (`min_length`, `max_length`, `min_size`, `max_size`) - can be violated during shrinking to find minimal counterexamples
  - Element constraints (from element generators) - should always be preserved

- **Float shrinking**: Float generators can shrink outside their original range (e.g., to 0.0, negative values) to find minimal counterexamples. This is expected behavior.

- **Nested structures**: Some generators are tested in nested contexts but not directly.
