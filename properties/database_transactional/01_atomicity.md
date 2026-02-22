# Atomicity Property

**Property**: Transactions are all-or-nothing. All operations in a transaction either commit together or rollback together.

## Concept

Test that transactions maintain atomicity: if any operation fails or transaction is rolled back, no partial changes are visible. Verify:
- Partial commits don't occur
- Rollback undoes all operations
- Failures during transaction don't leave partial state
- All-or-nothing behavior holds

**Key idea**: A transaction is an atomic unit; either all operations succeed or none do.

## Actions

Focus on transaction operations that might partially succeed:

- **Initial state generation**: Generate initial database state (empty, with existing data, at constraint boundaries, with pending transactions). Initial state determines what atomicity scenarios are possible.
- **Action sequences with parameters**: Generate transaction with N operations (N > 1), where each operation modifies different data items with specific parameters (values, keys, operation types). Action sequences test atomicity across multiple operations.
- **Multi-operation transactions**: Generate transactions with N operations (N > 1), where each operation modifies different data items
- **Simulated failures**: At operation K (1 ≤ K ≤ N), inject failure: throw exception, return error code, or simulate I/O failure. Verify no operations 1..K-1 are visible after failure
- **Rollback scenarios**: Explicit rollback calls, implicit rollbacks triggered by exceptions/errors
- **Nested transactions**: If supported, generate nested transaction hierarchies, test rollback propagation

**Concrete plan**:
1. Generate initial database state (data items, existing transactions, constraint states)
2. Generate transaction with N operations (each modifies distinct data item) with specific parameters
3. Capture initial state (all data items)
4. Execute operations 1..K-1 successfully
5. Inject failure at operation K
6. Verify rollback occurs
7. Capture final state
8. Compare initial vs final state (must be identical)

Verify atomicity:

- **No partial commits**: After rollback or failure, check all data items modified by transaction: none should show changes from operations 1..K-1
- **All-or-nothing**: If transaction commits, all N operations' changes visible. If rollback/failure, zero operations' changes visible
- **State rollback**: Compare state snapshot before transaction start vs after rollback: must be identical (byte-for-byte or value equality)
- **Failure handling**: After failure at operation K, verify: (1) rollback triggered automatically, (2) no data items show partial changes, (3) error propagated to caller

**Concrete checks**:
1. Before transaction: snapshot all data items that will be modified → state_before
2. After rollback/failure: snapshot same data items → state_after
3. Assert: state_before == state_after (all values identical)
4. If commit: assert all N operations' changes visible
5. If rollback/failure: assert zero operations' changes visible

## Subdomains

**Initial state: empty database**: Database starts empty. Action sequence: Single-operation transaction. Test: verify single-operation transactions always atomic (simplest case).

**Initial state: database with existing data**: Database starts with existing data items. Action sequence: Transaction with N operations where failure at operation K. Test: verify multi-operation cascading failures (no partial state visible), all operations 1..K-1 rolled back.

**Initial state: database supporting nested transactions**: Database starts with nested transaction support. Action sequence: Nested transaction hierarchies with inner rollback. Test: verify nested transaction rollback propagation (rollback propagates correctly), no partial commits at any level.

**Initial state: database ready for commit**: Database starts ready for commit operations. Action sequence: Operations succeed, then commit phase attempted. Test: verify failure at commit boundary (transaction treated as failed), no partial commit.

**Initial state: database with data**: Database starts with data. Action sequence: Long transaction with many operations (100+) where failure occurs near end. Test: verify long-running transaction failures (all operations rolled back), despite long execution time.

**Initial state: database with overlapping data**: Database starts with data that multiple transactions might modify. Action sequence: Transaction T1 rolling back while T2 committing, both modifying overlapping data. Test: verify concurrent rollback and commit (atomicity maintained), despite concurrency.
