# Database / Transactional Properties (ACID)

**Property**: Transactions satisfy ACID properties - Atomicity, Consistency, Isolation, Durability.

## Concept

Generate transaction sequences and verify ACID properties. Verify:
- Atomicity: All operations in transaction commit or all rollback
- Consistency: Transactions preserve database invariants
- Isolation: Concurrent transactions don't interfere
- Durability: Committed transactions persist despite failures

**Key idea**: Database transactions should maintain ACID guarantees even under concurrent access and failures.

## Actions

Focus on transaction operations:

- **Transaction operations**: Begin, commit, rollback, operations within transactions
- **Concurrent transactions**: Multiple transactions executing simultaneously
- **Failure injection**: Failures during transactions (to test atomicity/durability)
- **Isolation scenarios**: Transactions accessing same data concurrently

**Transaction sequences**: Generate sequences of transactions, vary isolation levels, inject failures.

## Checks

Verify ACID properties:

- **Atomicity**: Partial commits don't occur (all or nothing)
- **Consistency**: Database invariants preserved after transactions
- **Isolation**: Uncommitted changes not visible to other transactions
- **Durability**: Committed transactions persist after failures

**State verification**: Compare database state before/after transactions, verify isolation visibility rules.

## Subdomains

**Single transactions**: One transaction at a time. Baseline behavior.

**Concurrent transactions**: Multiple transactions, read/write conflicts, isolation scenarios.

**Failure scenarios**: Failures during transactions, recovery scenarios, durability under failures.
