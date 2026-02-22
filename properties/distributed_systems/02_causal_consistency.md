# Causal Consistency Property

**Property**: Causally related events seen in order. If event A causally precedes event B, all nodes see A before B.

## Concept

Test that distributed system maintains causal consistency: causally related events are seen in correct order by all nodes. Verify:
- Causal ordering preserved
- Causally related events seen in order
- No causal violations
- Causal chains maintained

**Key idea**: Causal relationships between events must be preserved across all nodes.

## Actions

Focus on causally related operations:

- **Causal chains**: Generate chain: node A writes X=1, node B reads X (sees 1), node B writes Y=2 (causally depends on reading X=1), node C reads Y. Track causal dependencies explicitly
- **Causal dependencies**: Track vector clocks or logical timestamps: when node B reads X written by A, B's clock incorporates A's clock. When B writes Y, Y's clock includes B's (which includes A's)
- **Causal ordering**: Generate operations: A writes X, B reads X then writes Y, C reads Y. Assert: all nodes see X before Y (causal order preserved)
- **Concurrent operations**: Generate concurrent writes: A writes X=1, B writes X=2 (no causal dependency). Assert: nodes can see these in any order

**Concrete plan**:
1. Track causal dependencies: when operation O1 reads data written by O2, mark O1 causally depends on O2
2. Generate operation sequence with causal dependencies
3. Execute operations on different nodes with network delays/partitions
4. At each node, record order of operations seen
5. Verify: if O1 causally depends on O2, all nodes see O2 before O1

## Checks

Verify causal consistency:

- **Causal ordering**: For each causal dependency (O1 depends on O2), check order at each node → assert all nodes see O2 before O1
- **No causal violations**: Check each node's operation log → assert no node sees O1 before O2 if O1 causally depends on O2
- **Causal chains**: For chain A→B→C (A causally before B, B causally before C), verify all nodes see A before B before C
- **Concurrent operations**: For concurrent operations (no causal dependency), verify nodes can see them in different orders (not required to be same)

**Concrete checks**:
1. Build causal dependency graph: O1 → O2 if O1 reads data written by O2
2. At each node, record operation order (list of operations in order seen)
3. For each causal dependency O1 → O2: check all nodes → assert O2 appears before O1 in all operation lists
4. Count violations: if any node sees O1 before O2 when O1 → O2, violation detected → assert zero violations
5. For concurrent operations (no path in dependency graph): allow different orders across nodes

## Subdomains

**Initial state: empty distributed system**: System starts empty (no data, no causal dependencies). Action sequence: A writes X, B reads X then writes Y. Test: verify simple causal chains (all nodes see X before Y), causal order preserved.

**Initial state: nodes with value V**: Nodes start with value V. Action sequence: Multiple nodes read same value V, then each writes different value. Test: verify fork-join causal patterns (all writes causally follow the read), ordering preserved.

**Initial state: nodes with different keys**: Nodes start with data on different keys (no causal dependencies). Action sequence: Two nodes write to different keys concurrently. Test: verify concurrent writes with no causality (nodes can see these in any order), no ordering required.

**Initial state: nodes with no causal chain**: Nodes start with no causal relationships. Action sequence: Long causal chain A→B→C→D where network reordering might cause C to be seen before B. Test: verify causal chain violations (causal order preserved), despite reordering.

**Initial state: nodes with value V**: Nodes start with value V. Action sequence: Node reads value V, modifies it, writes back. Test: verify read-modify-write causality (read-modify-write seen as atomic causal unit), by all nodes.

**Initial state: system partitioned**: System starts partitioned. Action sequence: Causal dependencies created across partition boundaries. Test: verify causal consistency with partitions (causal order preserved), even when messages delayed/dropped, eventual consistency maintained.
