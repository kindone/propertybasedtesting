# Thread Safety Property

**Property**: Safe concurrent access from multiple threads. Concurrent operations maintain invariants, no data races, no deadlocks.

## Concept

Execute concurrent action sequences from multiple threads. Verify:
- No data races occur
- No deadlocks occur
- Component invariants hold after concurrent execution
- Component state is consistent across threads
- Concurrent operations complete successfully

**Key idea**: Multiple threads can access component simultaneously; component should maintain correctness and avoid races/deadlocks.

## Actions

Focus on concurrent access patterns:

- **Read-read**: Multiple threads reading simultaneously (should be safe)
- **Read-write**: Some threads reading while others write (potential race)
- **Write-write**: Multiple threads writing simultaneously (potential race)
- **Lock contention**: Operations that acquire locks (potential deadlock)

**Concurrency model**: Generate operations for multiple threads, execute concurrently. Vary thread counts and operation mixes.

## Checks

After concurrent execution, verify:

- **Race detection**: Use TSAN to detect data races
- **Deadlock detection**: All threads complete (no infinite waiting)
- **Invariant preservation**: Component invariants hold (e.g., size ≥ 0)
- **State consistency**: Final state is consistent (e.g., size matches elements)

**Concurrent correctness**: Verify final state reflects all successful operations correctly, no lost updates.

## Subdomains

**Sequential baseline**: Single-threaded execution. Normal operation.

**Concurrent access**: Multiple threads, read/write mixes, operations that should be synchronized.

**Concurrency stress**: Many threads, high contention, lock contention, potential deadlock scenarios, timing-sensitive operations.
