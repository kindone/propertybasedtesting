# Partition Tolerance (CAP) Property

**Property**: System continues despite network partitions. System operates even when network is partitioned.

## Concept

Test that distributed system maintains partition tolerance: system continues to operate even when network is partitioned. Verify:
- System operates during partitions
- Partition tolerance maintained
- System handles partitions correctly
- Partition tolerance works

**Key idea**: System must tolerate network partitions - this is a requirement for distributed systems.

## Actions

Focus on partition scenarios:

- **Initial state generation**: Generate initial distributed system state (all nodes connected, some partitions already active, nodes with different states). Initial state determines what partition tolerance scenarios are possible.
- **Action sequences with parameters**: Generate operation sequences with specific parameters (read/write operations, keys, values, timing). Action sequences test partition tolerance across different partition scenarios.
- **Simulated partitions**: Implement mock network layer: partition nodes into groups (e.g., {A,B} vs {C,D}), drop messages between groups. Messages within same group delivered normally
- **Operations during partitions**: Generate operations (read/write) during partition. Operations within same partition group should work. Operations across partitions may fail/block
- **Partition recovery**: After partition duration, restore message delivery (remove partition in mock layer). Verify system recovers (merges state, reconciles differences)
- **Partition handling**: Test how system handles partitions: (1) continues operating within partitions, (2) handles cross-partition operations (may fail/block), (3) recovers after partition heals

**Concrete plan**:
1. Generate initial distributed system state (all nodes connected, or partitions active, or nodes with different states)
2. Implement mock network layer with partition configuration
3. Generate operation sequences with parameters (read/write operations, keys, values)
4. Create partition: split nodes into groups, drop messages between groups
5. Generate operations during partition: operations within same group, operations across groups
6. Verify: system continues operating (within partitions)
7. Heal partition: restore message delivery
8. Verify: system recovers, reconciles state differences

Verify partition tolerance:

- **Partition operation**: During partition, send operations to nodes within same partition group → assert operations succeed. System continues operating despite partition
- **Partition handling**: During partition: (1) operations within partition succeed, (2) cross-partition operations may fail/block (expected), (3) system doesn't crash. Assert: system handles partition gracefully
- **Recovery**: After partition heals, verify: (1) system reconciles state (all nodes eventually consistent), (2) no data loss, (3) operations resume normally. Check state consistency after recovery period
- **Tolerance maintained**: System operates despite partitions (doesn't require partition-free network). Assert: system designed for partition tolerance (doesn't assume perfect network)

**Concrete checks**:
1. Create partition, send operations within partition → assert operations succeed
2. Send cross-partition operations → assert: either fail gracefully OR block (not crash)
3. Heal partition, wait for recovery period (with timeout)
4. Verify state consistency: all nodes eventually see same state (or eventual consistency)
5. Verify no data loss: compare state before partition vs after recovery
6. Assert: system tolerates partitions (continues operating, recovers after heal)

## Subdomains

**Initial state: system with equal-sized node groups**: System starts with equal-sized node groups. Action sequence: Network partitions into two equal-sized groups, both think they're majority. Test: verify split-brain scenarios (system handles split-brain), only one partition accepts writes, or both block.

**Initial state: system with majority and minority groups**: System starts with majority and minority groups. Action sequence: Small partition isolated from majority. Test: verify minority partition isolation (minority partition handles requests appropriately), may block writes, serve reads from stale data.

**Initial state: system partitioned with conflicting writes**: System starts partitioned with partitions having written conflicting values. Action sequence: Partition heals, nodes reconcile. Test: verify partition healing with conflicts (conflict resolution), state reconciliation, no data loss.

**Initial state: system with network issues**: System starts with network issues that can spread. Action sequence: One partition triggers another (network issues spread). Test: verify cascading partition failures (system handles cascading partitions), continues operating.

**Initial state: system with asymmetric network**: System starts with asymmetric network conditions. Action sequence: Partition where one side can send to other but not receive (one-way communication). Test: verify asymmetric partitions (system handles asymmetric partitions correctly).

**Initial state: system with nodes that can fail**: System starts with nodes that can fail. Action sequence: Network partitions AND nodes fail simultaneously. Test: verify partition with node failures (system tolerates both partition and failures), continues operating where possible.
