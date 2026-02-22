# Safety Property

**Property**: Nothing bad happens. No data races, no corruption, no invalid states.

## Concept

Test that concurrent system maintains safety: no bad things happen during concurrent execution. Verify:
- No data races occur
- No state corruption
- No invalid states
- Invariants preserved

**Key idea**: System should maintain correctness (no races, corruption) during concurrent execution.

## Actions

Focus on concurrent access patterns:

- **Initial state generation**: Generate initial data structure state (empty, partially filled, full, at capacity, with specific elements). Initial state determines what race/corruption scenarios are possible.
- **Serial actions (setup)**: Set up data structure in specific state (insert elements, modify structure, prepare for concurrent access). Serial actions prepare the state space for concurrent testing.
- **Parallel actions (concurrent execution)**: Execute operations concurrently from multiple threads (inserts, deletes, modifications, queries). Parallel actions test safety guarantees.
- **Shared state access**: Generate operations from multiple threads accessing same shared data structure (e.g., concurrent inserts into map, concurrent increments of counter)
- **Race scenarios**: Generate unsynchronized access patterns (e.g., thread1 reads X, thread2 writes X without lock) → potential race
- **Corruption scenarios**: Generate operations that might corrupt data structure (e.g., concurrent list insertions, concurrent tree modifications)
- **Invariant violations**: Generate operations that might violate invariants (e.g., concurrent operations that might break tree balance, list ordering)

**Concrete plan**:
1. Generate initial data structure state (empty, size N, specific elements, at boundaries)
2. Serial actions: Set up data structure (insert elements, modify to reach desired state)
3. Parallel actions: Execute concurrent operations on shared data structure (multiple threads)
4. After all operations complete, verify data structure integrity
5. Use TSAN (ThreadSanitizer) to detect data races
6. Verify invariants hold (e.g., tree balanced, list ordered, counter value correct)

Verify safety:

- **No data races**: Run with ThreadSanitizer (TSAN) → assert no race conditions reported. TSAN detects: unsynchronized read-write or write-write on same memory location
- **No corruption**: After concurrent operations, verify data structure integrity: (1) parse/validate structure (e.g., tree is valid BST), (2) check for corruption (e.g., null pointers, cycles, invalid links)
- **No invalid states**: Check invariants after each operation (or periodically): assert invariants always hold (e.g., tree balanced, list ordered, counter >= 0)
- **Invariant preservation**: Define invariants explicitly (e.g., "tree is balanced", "list is sorted"). After concurrent execution, verify all invariants hold

**Concrete checks**:
1. Run with TSAN → assert zero race conditions reported
2. After concurrent operations: validate data structure (check all pointers valid, structure consistent)
3. Verify invariants: check each invariant predicate → assert all true
4. Compare expected vs actual: if operations were sequential, compute expected final state, compare with actual → assert matches (or within expected bounds for non-deterministic operations)
5. Assert: no races (TSAN), no corruption (structure valid), invariants hold

## Subdomains

**Initial state: counter at zero**: Counter starts at 0. Serial actions: None needed. Parallel actions: Multiple threads increment/decrement concurrently. Test: verify final value correct (no lost increments), TSAN detects races.

**Initial state: data structure empty**: Data structure starts empty. Serial actions: None needed. Parallel actions: Multiple threads insert/delete concurrently. Test: verify structure remains valid (no corruption, all elements present), invariants preserved.

**Initial state: data structure partially filled**: Data structure starts with some elements. Serial actions: Set up specific state (e.g., tree partially balanced, list partially sorted). Parallel actions: Multiple threads modify concurrently. Test: verify structure integrity maintained, invariants preserved.

**Initial state: data structure at capacity**: Data structure starts full (at capacity). Serial actions: Set up full state. Parallel actions: Multiple threads attempt insertions/deletions concurrently. Test: verify capacity handling, no corruption, invariants preserved.

**Initial state: data structure in rebalancing state**: Data structure starts in state requiring rebalancing (e.g., tree unbalanced, hash table needs resize). Serial actions: Trigger rebalancing condition. Parallel actions: Multiple threads access during rebalancing. Test: verify invariant violation windows handled correctly, no visible violations.

**Initial state: memory in use**: Memory allocated and in use. Serial actions: Set up memory access patterns. Parallel actions: One thread frees memory while another accesses it. Test: verify memory reclamation races handled, no use-after-free.

**Initial state: value at A**: Value starts at A. Serial actions: Set up value A. Parallel actions: Thread1 reads A, Thread2 changes A→B→A, Thread1 CAS succeeds incorrectly. Test: verify ABA problem scenarios, handled with version numbers/hazard pointers.
