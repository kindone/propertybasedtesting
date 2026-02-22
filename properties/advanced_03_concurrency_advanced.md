# Concurrency / Parallel Computing Properties

**Property**: Concurrent execution maintains correctness properties - serializability, liveness, safety, wait-freedom.

## Concept

Execute concurrent operations and verify concurrency properties. Verify:
- Serializability: Concurrent execution equivalent to some serial execution
- Liveness: System makes progress (no deadlocks, starvation, livelocks)
- Safety: Nothing bad happens (no data races, no corruption)
- Wait-freedom: Operations complete in finite steps

**Key idea**: Concurrent systems should maintain correctness properties while allowing parallel execution.

## Actions

Focus on concurrent execution patterns:

- **Concurrent operations**: Operations executed in parallel
- **Lock acquisition**: Operations that acquire locks (potential deadlock)
- **Shared state access**: Operations accessing shared state (potential races)
- **Synchronization points**: Operations at synchronization boundaries

**Concurrency patterns**: Generate concurrent operation sequences, vary thread counts, vary operation mixes.

## Checks

Verify concurrency properties:

- **Serializability**: Find equivalent serial execution order
- **Liveness**: All threads make progress (no deadlocks, starvation)
- **Safety**: No data races, no corruption, invariants hold
- **Wait-freedom**: Operations complete regardless of other threads

**Concurrency verification**: Use TSAN for race detection, verify progress, check for deadlocks.

## Subdomains

**Sequential baseline**: Single-threaded execution. Normal operation.

**Concurrent execution**: Multiple threads, read/write mixes, operations that should be synchronized.

**Concurrency stress**: Many threads, high contention, lock contention, potential deadlock scenarios, starvation scenarios.
