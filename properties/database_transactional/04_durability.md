# Durability Property

**Property**: Committed transactions persist despite failures. Once committed, transaction changes survive system failures.

## Concept

Test that transactions maintain durability: committed changes persist even after failures. Verify:
- Committed transactions survive failures
- Recovery restores committed state
- Uncommitted transactions not persisted
- Data persists across restarts

**Key idea**: Once a transaction commits, its changes are permanent and survive failures.

## Actions

Focus on failure and recovery scenarios:

- **Initial state generation**: Generate initial database state (empty, with existing committed data, with uncommitted transactions, with pending flushes). Initial state determines what durability scenarios are possible.
- **Action sequences with parameters**: Generate transaction sequences with operations that have specific parameters (commit operations, rollback operations, operations at different persistence stages). Action sequences test durability across different failure points.
- **Simulated failures**: Inject failures at specific points: (1) during write to buffer, (2) during flush to disk, (3) during commit log write. Use mock I/O layer or exception injection
- **Persistence verification**: After commit, directly read from durable storage (disk file, WAL log) to verify data written. Compare with in-memory state
- **Recovery logic**: If system has recovery: simulate crash (stop system), restart, verify recovery code executes and restores committed state
- **Commit persistence**: After commit, verify: (1) data in durable storage, (2) data recoverable after simulated crash
- **Rollback persistence**: After rollback, verify: (1) data NOT in durable storage, (2) data NOT recoverable after simulated crash

**Concrete plan**:
1. Generate initial database state (empty, with committed data, with uncommitted transactions)
2. Generate transaction sequence with commit/rollback operations (with parameters)
3. Commit transaction, capture committed data
4. Read directly from durable storage (WAL, data files) → verify data present
5. Simulate crash: stop system, clear in-memory state
6. Restart system, run recovery (if applicable)
7. Query recovered state → assert committed data present, uncommitted data absent

Verify durability:

- **Commit persistence**: After commit, read durable storage directly (parse WAL log, read data files) → assert committed data present with correct values
- **Persistence verification**: After simulated crash and recovery, query system state → assert all committed transactions' data present, all uncommitted transactions' data absent
- **No loss**: Compare committed data before crash vs after recovery → assert identical (no data loss)
- **Rollback persistence**: After rollback, read durable storage → assert no data from rolled-back transaction present
- **Recovery correctness**: After recovery, verify: (1) committed transactions present (query and verify values), (2) uncommitted transactions absent (query and verify not present)

**Concrete checks**:
1. After commit: read WAL log → assert commit record present with transaction data
2. After commit: read data files → assert data written with correct values
3. Simulate crash, restart, recover
4. Query recovered state → assert committed data matches pre-crash committed data
5. Query recovered state → assert uncommitted data not present
6. Compare checksums/hashes of committed data before/after recovery → assert identical

## Subdomains

**Initial state: database with committed data**: Database starts with existing committed data. Action sequence: Transaction commits successfully in memory, but WAL write fails. Test: verify commit log write failures (transaction treated as not committed), data not persisted.

**Initial state: database with multiple transactions**: Database starts with multiple transactions in various states. Action sequence: Multiple transactions commit, but only some data flushed to disk before crash. Test: verify partial flush scenarios (only flushed transactions recoverable), unflushed lost.

**Initial state: database during checkpoint**: Database starts during checkpoint operation (dirty pages being written). Action sequence: System crashes while checkpointing. Test: verify crash during checkpoint (recovery handles partial checkpoint correctly).

**Initial state: database with concurrent operations**: Database starts with concurrent commit and flush operations. Action sequence: Transaction commits while another transaction's data being flushed, then crash occurs. Test: verify concurrent commit and crash (committed transaction persists), partial flush handled.

**Initial state: database with storage near capacity**: Database starts with storage near full capacity. Action sequence: Transaction commits, but disk full during write. Test: verify storage full scenarios (transaction fails gracefully), no partial write, system remains consistent.

**Initial state: database with committed transactions**: Database starts with committed transactions. Action sequence: System crashes, remains down for extended period, then recovers. Test: verify recovery after long downtime (committed transactions still recoverable), no data corruption.
