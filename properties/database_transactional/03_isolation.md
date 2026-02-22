# Isolation Property

**Property**: Concurrent transactions don't interfere. Uncommitted changes are only visible to the transaction that made them.

## Concept

Test that transactions maintain isolation: concurrent transactions don't see each other's uncommitted changes. Verify:
- Uncommitted changes not visible to other transactions
- Isolation level guarantees hold
- No interference between concurrent transactions
- Visibility rules followed

**Key idea**: Transactions should be isolated from each other until they commit.

## Actions

Focus on concurrent transaction scenarios:

- **Initial state generation**: Generate database initial state (empty, with existing data, with pending transactions, at constraint boundaries). Initial state determines what concurrent scenarios are possible.
- **Serial actions (setup)**: Set up transaction T1 in specific state (start T1, perform operations to reach desired state, leave uncommitted). Serial actions prepare the state space for concurrent testing.
- **Parallel actions (concurrent execution)**: Start transaction T2 concurrently with T1, perform operations on same data items. Parallel actions test isolation guarantees.
- **Isolation levels**: Test each isolation level separately: read uncommitted, read committed, repeatable read, serializable. Different levels allow different visibility patterns.
- **Visibility scenarios**: Generate sequence: T1 writes X (serial), T2 reads X concurrently (parallel), T1 commits (serial), T2 reads X again (parallel). Test visibility at each step.

**Concrete plan**:
1. Generate initial database state (data items, existing transactions, constraint states)
2. Serial actions: Start transaction T1, perform operations (read/write data item X), leave uncommitted
3. Parallel actions: Start transaction T2 concurrently, perform operations on same X
4. Capture what T2 sees at each point (before/after T1 commits)
5. Serial actions: Commit/rollback T1, then T2
6. Verify visibility matches isolation level guarantees

Verify isolation:

- **Uncommitted invisibility**: T1 writes X (uncommitted), T2 reads X → assert T2 sees old value (not T1's write) for read committed/repeatable read/serializable
- **Isolation level compliance**:
  - Read uncommitted: T2 can see T1's uncommitted writes (dirty reads allowed)
  - Read committed: T2 cannot see T1's uncommitted writes (no dirty reads)
  - Repeatable read: T2 reads X twice, sees same value even if T1 commits between reads (no non-repeatable reads)
  - Serializable: No phantom reads (T2's query returns same rows even if T1 inserts matching rows)
- **No interference**: Both transactions complete successfully OR one blocks until other completes
- **Visibility rules**: Track read results at each step, assert they match isolation level specification

**Concrete checks**:
1. T1 writes X=value1 (uncommitted), T2 reads X → assert T2 sees old_value (for read committed+)
2. T1 commits, T2 reads X again → assert T2 sees value1 (for read committed)
3. T2 reads X twice (before/after T1 commits) → assert same value (for repeatable read+)
4. T1 inserts row matching T2's query, T2 queries again → assert same row count (for serializable)

## Subdomains

**Initial state: empty database**: Database starts empty. Serial actions: T1 inserts data. Parallel actions: T2 reads/writes same data. Test: verify isolation maintained from initial empty state.

**Initial state: database at constraint boundaries**: Database starts with data at constraint limits (e.g., balance = 0, inventory = 0). Serial actions: T1 attempts operations near boundaries. Parallel actions: T2 reads/modifies same constrained data. Test: verify isolation maintained even at boundaries.

**Initial state: database with pending transactions**: Database starts with existing uncommitted transactions. Serial actions: T1 performs operations that interact with pending state. Parallel actions: T2 sees/isolation from pending transactions. Test: verify isolation from existing pending transactions.

**Dirty read scenarios**: Initial state: database with data. Serial actions: T1 writes X (uncommitted). Parallel actions: T2 reads X. Serial actions: T1 rolls back. Test: verify dirty reads prevented (read committed+) or allowed (read uncommitted).

**Lost update patterns**: Initial state: database with value X. Serial actions: T1 reads X. Parallel actions: T2 reads X, writes X+1, commits. Serial actions: T1 writes X+1, commits. Test: verify lost updates prevented (serializable) or allowed (lower isolation levels).

**Non-repeatable read sequences**: Initial state: database with data. Serial actions: T1 reads X. Parallel actions: T2 modifies X, commits. Serial actions: T1 reads X again. Test: verify non-repeatable reads prevented (repeatable read+) or allowed (read committed).

**Phantom read scenarios**: Initial state: database with some rows. Serial actions: T1 queries matching condition. Parallel actions: T2 inserts matching row, commits. Serial actions: T1 queries again. Test: verify phantom reads prevented (serializable) or allowed (repeatable read).
