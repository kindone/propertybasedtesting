# Eventual Consistency Property

**Property**: System converges to consistent state. All nodes eventually see the same data.

## Concept

Test that distributed system maintains eventual consistency: despite network delays and partitions, system converges to consistent state. Verify:
- All nodes eventually see same state
- Convergence happens after network stabilizes
- No permanent inconsistencies
- System reaches consistent state

**Key idea**: System may be temporarily inconsistent, but eventually all nodes converge to same state.

## Actions

Focus on distributed operations and network conditions:

- **Initial state generation**: Generate initial distributed system state (all nodes consistent, nodes with different values, nodes with pending operations, network partitions already active). Initial state determines what convergence scenarios are possible.
- **Action sequences with parameters**: Generate operation sequences with write/read operations that have specific parameters (values, keys, timing). Action sequences test eventual consistency across different network conditions.
- **Operations across nodes**: Generate write operation on node A, read operations on nodes B, C, D. Track what each node sees over time
- **Simulated network delays**: Implement mock network layer that delays messages by T milliseconds (T chosen randomly). Messages arrive out of order
- **Simulated partitions**: Implement mock network layer that drops messages between node groups (e.g., {A,B} vs {C,D}). Messages within group delivered, between groups dropped
- **Convergence scenarios**: Generate writes on different nodes, simulate network conditions, then stabilize network (restore delivery), verify all nodes converge

**Concrete plan**:
1. Generate initial distributed system state (all nodes consistent, nodes with different values, partitions active)
2. Implement mock network layer: intercept messages, apply delays/drops based on partition configuration
3. Generate operation sequences: node A writes value V1, node B writes value V2 (with specific parameters)
4. Simulate network: delay messages, create partitions
5. Query all nodes periodically: record what each node sees at time T
6. Stabilize network: restore normal delivery
7. Wait for convergence period (with timeout)
8. Query all nodes: assert all see same final state

Verify eventual consistency:

- **Convergence**: After network stabilizes, wait up to timeout T (e.g., 10 seconds). Query all nodes every 100ms. Assert: within T, all nodes return same value for same key
- **No permanent inconsistencies**: Track state at each node over time. Assert: if network stable for duration D, all nodes converge within D
- **Stable state**: After network stabilizes, query all nodes → assert all return identical state (same keys, same values)
- **Eventual agreement**: Compare state snapshots from all nodes after convergence timeout → assert all identical (same key-value pairs)

**Concrete checks**:
1. After network stabilizes, start timer
2. Every 100ms: query all nodes for same key K → record values
3. Within timeout T: assert all nodes return same value V for key K
4. After convergence: snapshot state from all nodes → assert all identical
5. Verify no node stuck with stale value indefinitely

## Subdomains

**Initial state: all nodes consistent**: All nodes start with same values. Action sequence: Multiple nodes write to same key while partitioned. Test: verify write conflicts during partition (conflict resolution: last-write-wins, vector clocks, CRDTs), all nodes converge to same value.

**Initial state: empty distributed system**: System starts empty (no data). Action sequence: Node A writes value, then immediately reads same key. Test: verify read-your-writes consistency (node A sees its own write immediately), even if other nodes don't yet.

**Initial state: nodes with value V1**: Nodes start with value V1. Action sequence: Node reads value V1, then reads again later. Test: verify monotonic reads (second read sees V1 or newer value), never older, even if network reorders.

**Initial state: nodes with no causal dependencies**: Nodes start with no causal relationships. Action sequence: Node A writes X, node B reads X then writes Y (causally depends on X), node C reads Y. Test: verify causal consistency violations (C sees X before Y), causal order preserved.

**Initial state: system partitioned with conflicting values**: System starts partitioned with nodes having conflicting values. Action sequence: Partition heals, nodes reconcile. Test: verify partition healing with conflicts (conflict resolution), all nodes converge to consistent state.

**Initial state: nodes with stale values**: Nodes start with stale values (not seeing latest updates). Action sequence: Node reads value, network delays prevent seeing updates, node reads again after long delay. Test: verify stale read scenarios (eventually sees updated value), no permanent staleness.
