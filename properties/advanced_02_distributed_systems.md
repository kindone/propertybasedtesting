# Distributed Systems Properties

**Property**: System maintains consistency model (eventual, causal, linearizable). Operations appear atomic and ordered correctly.

## Concept

Generate distributed operation sequences across multiple nodes. Verify:
- Consistency model holds (eventual, causal, linearizable)
- Operations appear atomic and in correct order
- Causally related events seen in order
- System converges to consistent state

**Key idea**: Distributed systems should maintain their consistency model despite network delays, partitions, and node failures.

## Actions

Focus on distributed operations:

- **Operations across nodes**: Operations executed on different nodes
- **Network partitions**: Simulate network partitions between nodes
- **Causal operations**: Operations that are causally related
- **Concurrent operations**: Operations happening concurrently across nodes

**Distributed sequences**: Generate operations for multiple nodes, simulate network conditions, vary timing.

## Checks

Verify consistency model:

- **Eventual consistency**: System converges to consistent state
- **Causal consistency**: Causally related events seen in order
- **Linearizability**: Operations appear atomic and in some total order
- **Ordering**: Operations seen in correct order by all nodes

**Consistency verification**: Check operation ordering, verify all nodes see consistent state eventually.

## Subdomains

**Normal operation**: No partitions, normal network conditions. Baseline behavior.

**Network partitions**: Partitions between nodes, operations during partitions, recovery after partitions.

**Consistency stress**: Many concurrent operations, high latency, node failures, partition scenarios.
