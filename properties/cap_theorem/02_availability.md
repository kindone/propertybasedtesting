# Availability (CAP) Property

**Property**: System remains operational. System responds to requests even during failures.

## Concept

Test that distributed system maintains availability: system responds to requests even during failures. Verify:
- System responds to requests
- No unavailability periods
- System remains operational during failures
- Availability maintained

**Key idea**: System prioritizes availability - system responds to requests, even if consistency is sacrificed.

## Actions

Focus on system availability:

- **Initial state generation**: Generate initial distributed system state (all nodes operational, some nodes failed, partitions already active). Initial state determines what availability scenarios are possible.
- **Action sequences with parameters**: Generate request sequences with specific parameters (request types, keys, values, timing). Action sequences test availability across different failure/partition scenarios.
- **Request operations**: Generate read/write requests to system. Send requests continuously (e.g., every 100ms)
- **Simulated failures**: Simulate node failure: stop node A (mark as failed in mock layer). Generate requests → verify system still responds (from other nodes)
- **Simulated partitions**: Implement mock network layer: partition nodes into groups, drop messages between groups. Generate requests → verify system responds (may return stale data but responds)
- **Availability scenarios**: Test during failures/partitions: send requests, measure response rate → assert system responds to requests (may sacrifice consistency)

**Concrete plan**:
1. Generate initial distributed system state (all nodes operational, or some failed, or partitions active)
2. Implement mock network layer with failure/partition simulation
3. Generate request sequences with parameters (continuous stream of requests: read/write operations, keys, values)
4. Simulate failure: mark node A as failed
5. Send requests → verify system responds (from other nodes)
6. Simulate partition: partition nodes, drop messages between partitions
7. Send requests → verify system responds (may return stale data, but responds)
8. Measure: response rate should remain high (e.g., >95% requests get response)

Verify availability:

- **Request response**: Send N requests (e.g., 100 requests). Count responses received within timeout T (e.g., 1 second) → assert response rate > threshold (e.g., >95%)
- **No unavailability**: Monitor system continuously: send request every 100ms, track responses → assert no period > P (e.g., 1 second) with zero responses
- **Failure tolerance**: Simulate node failure, send requests → assert system responds (from other nodes). Response rate should remain high (e.g., >90%)
- **Availability guarantee**: During partition, send requests → assert system responds (may return stale data, but responds). Response rate should remain high (e.g., >95%)

**Concrete checks**:
1. Send N requests (e.g., 100), measure responses within timeout T
2. Calculate response rate: responses / requests → assert > threshold (e.g., 0.95)
3. During failure: send requests → assert response rate remains high
4. During partition: send requests → assert response rate remains high (may get stale data)
5. Monitor continuously: no period > P with zero responses
6. Assert: system prioritizes availability (responds even if consistency sacrificed)

## Subdomains

**Initial state: system with all nodes operational**: System starts with all nodes operational. Action sequence: One or more nodes fail, but system continues responding. Test: verify availability during node failures (requests routed to available nodes), response rate remains high.

**Initial state: system partitioned**: System starts partitioned. Action sequence: System returns stale data during partition to maintain availability. Test: verify availability with stale data (system responds even with stale data), no unavailability periods.

**Initial state: system with core and degraded features**: System starts with core and degraded features available. Action sequence: Failures occur, some features unavailable. Test: verify graceful degradation (core functionality available), degraded features handled gracefully.

**Initial state: system with nodes that can fail/recover**: System starts with nodes that can fail and recover. Action sequence: Nodes fail and recover rapidly. Test: verify rapid failure recovery (system adapts quickly), availability maintained during transitions.

**Initial state: system partitioned**: System starts partitioned. Action sequence: Requests sent during partition. Test: verify availability vs consistency tradeoff (requests served: may return stale data), no blocking for consistency.

**Initial state: system spanning multiple regions**: System starts spanning multiple regions. Action sequence: Some regions fail. Test: verify multi-region availability (requests served from available regions), no global unavailability.
