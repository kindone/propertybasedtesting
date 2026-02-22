# Consistency Property

**Property**: Internal invariants maintained across all operations. Component-specific invariants hold in all states (e.g., "size ≥ 0", "no cycles", "sorted").

## Concept

Execute action sequences and verify component-specific invariants hold. Verify:
- Invariants hold after each action
- Invariants hold across all state transitions
- Invariants are not violated by any operation
- Component maintains its structural properties

**Key idea**: Component should maintain its invariants throughout its lifetime; no operation should violate them.

## Actions

Focus on operations that might affect invariants:

- **Operations that modify structure**: Insert, delete, reorder (may affect structural invariants)
- **Operations that change size**: Grow, shrink (may affect size invariants)
- **Operations that modify ordering**: Sort, reorder (may affect ordering invariants)
- **Operations on edge states**: Operations when empty, full, partially filled

**Invariant checking**: Check invariants after each operation, especially after operations that might violate them.

## Checks

Verify invariants hold:

- **Size invariants**: Size ≥ 0, size matches elements, capacity ≥ size
- **Structural invariants**: No cycles, tree properties hold, list structure valid
- **Ordering invariants**: Sorted, heap property, etc.
- **Component-specific invariants**: Invariants specific to component type

**Invariant verification**: Check each invariant directly (e.g., traverse structure, verify properties).

## Subdomains

**Normal operations**: Operations that preserve invariants, typical usage patterns.

**Invariant-challenging operations**: Operations that might violate invariants, edge cases, boundary conditions.

**Structural stress**: Many operations, rapid operations, operations that stress structural properties.
