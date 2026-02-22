# Algorithms / Data Structures Property

**Property**: Algorithm produces correct output, meets complexity bounds, maintains data structure invariants.

## Concept

Test algorithms and data structures against their specifications. Verify:
- Algorithm produces correct output for all inputs
- Time/space complexity meets stated bounds
- Data structure invariants are maintained
- Stability properties hold (for sorting algorithms)

**Key idea**: Algorithms and data structures should be correct and meet their complexity guarantees.

## Actions

Focus on algorithm/data structure operations:

- **Algorithm execution**: Run algorithm on generated inputs
- **Data structure operations**: Insert, delete, query operations on data structures
- **Complexity stress tests**: Large inputs to verify complexity bounds
- **Invariant-challenging operations**: Operations that might violate data structure invariants

**Input generation**: Generate inputs of varying sizes, edge cases, typical cases.

## Checks

Verify correctness and complexity:

- **Correctness**: Algorithm output is correct (compare against reference or verify properties)
- **Complexity bounds**: Execution time/space within stated bounds (O(n), O(n log n), etc.)
- **Invariants**: Data structure invariants hold (heap property, BST ordering, etc.)
- **Stability**: For sorting, relative order of equal elements preserved

**Reference comparison**: Compare against known-correct implementation or verify mathematical properties.

## Subdomains

**Normal inputs**: Typical inputs, normal sizes, well-formed data.

**Edge cases**: Empty input, single element, sorted/reverse-sorted, duplicates, boundary values.

**Complexity stress**: Large inputs, worst-case inputs, inputs that stress complexity bounds.
