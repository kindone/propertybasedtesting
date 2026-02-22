# Serializability Property

**Property**: Concurrent execution equivalent to some serial execution. There exists a serial order that produces the same result.

## Concept

Test that concurrent execution is serializable: find a serial execution order that produces the same result. Verify:
- Equivalent serial execution exists
- Result matches some serial order
- No non-serializable anomalies
- Serialization order valid

**Key idea**: Concurrent execution should be equivalent to executing operations in some sequential order.

## Actions

Focus on concurrent operations:

- **Initial state generation**: Generate initial data structure state (empty, with specific values, at boundaries). Initial state determines what conflict scenarios are possible.
- **Serial actions (setup)**: Set up data structure in specific state (insert elements, modify to create conflict conditions). Serial actions prepare the state space for concurrent testing.
- **Parallel actions (concurrent execution)**: Execute operation sequences concurrently from multiple threads (e.g., thread1: [read X, write Y], thread2: [write X, read Y]). Parallel actions test serializability.
- **Operation sequences**: Each thread executes sequence of operations with parameters (e.g., thread1: [read X, write Y], thread2: [write X, read Y])
- **Conflict scenarios**: Identify conflicts: (1) read-write: thread1 reads X, thread2 writes X, (2) write-write: both threads write X
- **Serialization points**: Track when each operation appears to execute (serialization point)

**Concrete plan**:
1. Generate initial data structure state (empty, with values, at boundaries)
2. Serial actions: Set up data structure (insert elements, modify to create conflict conditions)
3. Generate operation sequences for N threads (each thread has list of operations with parameters)
4. Parallel actions: Execute operations concurrently (use actual threads or simulated concurrency)
5. Record final state after all operations complete
6. Track conflicts: which operations conflict (access same data item, at least one write)
7. Find serial order: try different serial orders of operations, check if any produces same final state

Verify serializability:

- **Anomaly detection**: Check for specific anomalies:
  - Dirty read: T1 writes X (uncommitted), T2 reads X, T1 rolls back → T2 saw uncommitted data
  - Lost update: T1 reads X, T2 reads X, T1 writes X+1, T2 writes X+1 → T1's update lost
  - Non-repeatable read: T1 reads X twice, sees different values (T2 modified X between reads)
  - Phantom read: T1 queries (returns rows), T2 inserts matching row, T1 queries again (returns more rows)
- **Result equivalence**: Capture final state after concurrent execution. Enumerate all serial orders (N! orders for N operations). For each serial order, execute operations sequentially, compare final state → assert one matches concurrent execution's final state
- **Conflict serialization**: For conflicting operations (read-write or write-write on same item), verify they're serialized (one executes completely before other starts, or proper locking prevents conflicts)
- **Practical verification**: For small sets (<8 operations), enumerate serial orders. For large sets, check for specific anomalies (dirty read, lost update, etc.)

**Concrete checks**:
1. Execute operations concurrently, capture final state S_concurrent
2. List all operations, enumerate all serial orders (permutations)
3. For each serial order: execute operations sequentially, capture final state S_serial
4. Assert: S_serial == S_concurrent for at least one serial order
5. Check for anomalies: dirty read, lost update, non-repeatable read, phantom read → assert none occur

## Subdomains

**Initial state: data structure with value X**: Data structure starts with value X. Serial actions: None needed. Parallel actions: Thread1 reads X, Thread2 writes X concurrently. Test: verify read-write conflict serialization (either read sees old value or write completes first), no dirty reads.

**Initial state: data structure with value X**: Data structure starts with value X. Serial actions: None needed. Parallel actions: Two threads write to same location concurrently. Test: verify write-write conflict serialization (one write completes before other starts, or both fail atomically), no lost updates.

**Initial state: data structure with data**: Data structure starts with data. Serial actions: None needed. Parallel actions: Multiple threads read same data concurrently (no writes). Test: verify read-only transaction isolation (all reads see consistent snapshot), no interference.

**Initial state: data structure partially filled**: Data structure starts partially filled. Serial actions: Set up mixed read/write conditions. Parallel actions: Complex patterns of reads and writes from multiple threads (e.g., readers-writers problem). Test: verify mixed read-write patterns serializable, no anomalies.

**Initial state: data structure with multiple items**: Data structure starts with multiple items (X, Y, Z). Serial actions: None needed. Parallel actions: Operations on different data items executed concurrently. Test: verify non-conflicting operation parallelism (can execute in parallel), no false conflicts.

**Initial state: data structure with values X, Y, Z**: Data structure starts with values X, Y, Z. Serial actions: Set up conflict conditions. Parallel actions: Operations that create cycles in conflict graph (e.g., T1 conflicts with T2, T2 conflicts with T3, T3 conflicts with T1). Test: verify conflict graph cycles handled (serialization order exists despite cycles).
