# Linearizability Property

**Property**: Operations appear atomic and in some total order. Each operation appears to execute atomically at some point between invocation and response.

## Concept

Test that distributed system maintains linearizability: operations appear to execute atomically in some total order consistent with real-time ordering. Verify:
- Operations appear atomic
- Total order exists
- Order consistent with real-time
- Atomicity maintained

**Key idea**: System should appear as if operations execute atomically in some sequential order.

## Actions

Focus on concurrent operations:

- **Concurrent operations**: Generate operations on same data item X from different nodes, execute concurrently (overlapping invocation/response times)
- **Real-time ordering**: Record timestamp of each operation's invocation (T_invoke) and response (T_response). Operations ordered by real-time: O1 before O2 if T_response(O1) < T_invoke(O2)
- **Atomic operations**: Each operation should appear to execute atomically at some point between T_invoke and T_response
- **Ordering scenarios**: Generate operations that should appear in specific order based on real-time (e.g., if O1 completes before O2 starts, all nodes should see O1's effects before O2's)

**Concrete plan**:
1. Generate concurrent operations on same data item X
2. Record T_invoke and T_response for each operation (use synchronized clock)
3. Execute operations concurrently across nodes
4. Record what each node sees (read results) at different times
5. Find linearization point for each operation (point between T_invoke and T_response)
6. Verify: if linearization points ordered, all nodes see effects in that order

Verify linearizability:

- **Anomaly detection**: Check for violations: if O1 completes (T_response(O1)) before O2 starts (T_invoke(O2)), but some node sees O2's effects before O1's → violation
- **Real-time consistency**: Build real-time order: O1 < O2 if T_response(O1) < T_invoke(O2). Verify: linearization order respects real-time order
- **Atomicity**: For each operation, verify: all nodes see operation's effects appear atomically (not partially). If operation writes value V, all nodes see either old value or V (never partial V)
- **Practical verification**: For small sets (<10 operations), enumerate all possible linearization orders, verify one matches observed behavior. For large sets, check for specific violations (read old value after write completes)

**Concrete checks**:
1. Record T_invoke and T_response for all operations
2. Build real-time order: O1 < O2 if T_response(O1) < T_invoke(O2)
3. For each node, record when it sees each operation's effects
4. Check violations: if O1 < O2 in real-time but node sees O2 before O1 → violation
5. For small sets: try all possible linearization orders, verify one matches all nodes' observations
6. Assert: no violations found, or linearization order exists

## Subdomains

**Initial state: register with value V**: Register starts with value V. Serial actions: None needed. Parallel actions: Concurrent read-modify-write operations (e.g., increment counter). Test: verify read-modify-write linearizability (operations appear atomic), final value matches some sequential execution.

**Initial state: register with value V**: Register starts with value V. Serial actions: None needed. Parallel actions: Concurrent reads and writes to single register. Test: verify register linearizability (each read returns value of most recent write), that completed before read started.

**Initial state: register with value V**: Register starts with value V. Serial actions: Operation O1 reads value V. Parallel actions: Operation O2 writes value W, O1 completes before O2 starts. Test: verify write-after-read ordering (all nodes see O1's read before O2's write).

**Initial state: register with value V**: Register starts with value V. Serial actions: Set up overlapping conditions. Parallel actions: Operations with overlapping invocation/response times. Test: verify overlapping operations (linearization points exist between invocation and response), ordering consistent.

**Initial state: register with value V**: Register starts with value V. Serial actions: Set up conditions for violations. Parallel actions: Operations that appear to execute out of real-time order (e.g., later operation's effects visible before earlier operation completes). Test: verify linearizability violations (violations detected).

**Initial state: register with value V**: Register starts with value V. Serial actions: Set up high contention conditions. Parallel actions: Many concurrent operations on same data item. Test: verify high contention linearizability (linearizability maintained despite high contention), no anomalies.
