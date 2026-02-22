# Memory Safety Property

**Property**: No memory leaks, double-free, use-after-free, buffer overflows. After action sequences and destruction, no memory leaks; no invalid memory access.

## Concept

Execute action sequences and track memory allocations/deallocations. Verify:
- All allocated memory is deallocated (no leaks)
- No double-free occurs
- No use-after-free occurs
- No buffer overflows occur
- Memory access is valid throughout component lifetime

**Key idea**: Component should manage memory correctly throughout its lifetime, including after destruction.

## Actions

Focus on memory allocation/deallocation patterns:

- **Growth operations**: Insert, push_back, resize (trigger allocations)
- **Shrinkage operations**: Erase, pop_back, shrink_to_fit (trigger deallocations)
- **Reallocation scenarios**: Operations that may reallocate (grow beyond capacity)
- **Destruction**: Component destruction releases all memory

**Allocation tracking**: Track each allocation and deallocation. Use allocation tracker or sanitizers (ASAN, MSAN).

## Checks

After sequences and destruction, verify:

- **Leak check**: Allocations match deallocations (no leaks)
- **Double-free check**: No memory deallocated twice
- **Use-after-free check**: No access to deallocated memory
- **Bounds check**: No buffer overflows (access within allocated bounds)

**Sanitizer verification**: Use ASAN/MSAN to detect memory errors. Verify allocation/deallocation counts match.

## Subdomains

**Normal allocation patterns**: Typical grow/shrink cycles, normal usage sequences.

**Allocation edge cases**: Empty component, single allocation, capacity reallocation, rapid grow/shrink cycles.

**Memory stress**: Many allocations, large allocations, allocation exhaustion, destruction after many operations.
