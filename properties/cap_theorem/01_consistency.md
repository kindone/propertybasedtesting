# Consistency (CAP) Property

**Property**: All nodes see same data simultaneously. All nodes return same data for same query.

## Concept

Test that distributed system maintains consistency: all nodes see same data at same time. Verify:
- All nodes return same data for same query
- No stale data visible
- Immediate consistency across nodes
- Consistency maintained

**Key idea**: System prioritizes consistency - all nodes see same data simultaneously, even if availability is sacrificed.

## Actions

Focus on distributed read operations:

- **Initial state generation**: Generate initial distributed system state (all nodes consistent, nodes with different values, partitions already active). Initial state determines what consistency scenarios are possible.
- **Serial actions (setup)**: Set up conditions for consistency testing (write values, create partitions, prepare nodes). Serial actions prepare the state space for concurrent testing.
- **Parallel actions (concurrent execution)**: Execute read/write operations concurrently across nodes with specific parameters (keys, values, timing). Parallel actions test consistency guarantees.
- **Read operations**: Generate read operation for key K from nodes A, B, C simultaneously (within small time window, e.g., 10ms)
- **Write operations**: Generate write operation: node A writes value V to key K, then immediately (within 10ms) read from nodes B, C
- **Consistency scenarios**: Test immediate consistency: after write on node A, all nodes should return same value immediately
- **Simulated partitions**: Implement mock network layer: partition nodes into groups (e.g., {A,B} vs {C,D}), drop messages between groups. Test: system may block/reject requests to maintain consistency

**Concrete plan**:
1. Generate initial distributed system state (all nodes consistent, or with different values, or partitions active)
2. Serial actions: Set up conditions (write values, create partitions, prepare nodes)
3. Implement mock network layer with partition configuration
4. Parallel actions: Write value V to key K on node A, immediately (within 10ms) read key K from all nodes B, C, D
5. Simulate partition: drop messages between {A,B} and {C,D}
6. Parallel actions: Write on A, read from B (same partition) → should see update
7. Parallel actions: Read from C, D (different partition) → may block or return error (consistency maintained)
8. Verify: all nodes in same partition see same value

Verify consistency:

- **Same data**: Write value V to key K on node A. Within 10ms, read K from all nodes → assert all return V (or error if partition prevents read)
- **No staleness**: Write V1 to K on A, then V2 to K on A. Read K from all nodes → assert all return V2 (never V1)
- **Immediate consistency**: Write V to K on A, immediately read K from B → assert B returns V (not old value)
- **Consistency guarantee**: During partition, if system maintains consistency: (1) writes may be rejected, (2) reads may block/timeout, (3) nodes in same partition see same value. Assert: no node sees inconsistent state

**Concrete checks**:
1. Write V to K on node A
2. Read K from all nodes simultaneously (within 10ms window) → assert all return V
3. During partition: write on A, read from B (same partition) → assert B sees write
4. During partition: read from C (different partition) → assert: either blocks/timeouts OR returns consistent value (not stale)
5. Assert: no two nodes see different values for same key at same time
6. Assert: system may sacrifice availability (reject/block requests) to maintain consistency

## Subdomains

**Consistency during partition**: Network partitions, but system maintains consistency. Test: verify writes blocked/rejected during partition, reads may block, no inconsistent state visible.

**Read consistency across nodes**: Multiple nodes read same key simultaneously during normal operation. Test: verify all return same value, no stale reads.

**Write propagation consistency**: Write on one node, immediately read from other nodes. Test: verify all nodes see write immediately (or block until consistent), no stale values.

**Consistency vs availability tradeoff**: During partition, system chooses consistency over availability. Test: verify requests blocked/rejected to maintain consistency, no inconsistent responses.

**Consistency with quorum**: System uses quorum for consistency (majority of nodes must agree). Test: verify quorum requirements enforced, consistency maintained even with minority partition.

**Stale read prevention**: System prevents stale reads even under load. Test: verify reads always return latest committed value, no read-your-writes violations.
