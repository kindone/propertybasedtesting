# Timeout Handling Property

**Property**: Connections/timeouts handled correctly. System handles timeouts and recovers appropriately.

## Concept

Test that protocol handles timeouts correctly: timeouts are detected and handled appropriately. Verify:
- Timeouts detected correctly
- Connections handled on timeout
- Recovery after timeout works
- Timeout behavior is correct

**Key idea**: Protocol should handle timeouts gracefully and recover appropriately.

## Actions

Focus on timeout scenarios:

- **Initial state generation**: Generate initial protocol state (connections established, no pending requests, some requests in flight, timeout timers initialized). Initial state determines what timeout scenarios are possible.
- **Action sequences with parameters**: Generate request/response sequences with specific parameters (timeout values, request types, response delays). Action sequences test timeout handling across different scenarios.
- **Timeout scenarios**: Simulate timeouts: (1) delay response beyond timeout threshold T (e.g., T=5 seconds), (2) don't send response at all (simulate failure)
- **Connection timeouts**: Test connection timeout: establish connection, don't send data for period > timeout → verify connection closed
- **Operation timeouts**: Test operation timeout: send request, delay response > timeout → verify operation times out
- **Recovery scenarios**: After timeout, verify: (1) connection/operation cleaned up, (2) system can retry or handle error, (3) system state consistent
- **Simulated timeouts**: Implement mock network layer: delay responses or drop responses. Use time control (mock clock) to simulate timeout expiration

**Concrete plan**:
1. Generate initial protocol state (connections established, timeout timers initialized)
2. Implement mock network layer with configurable response delays
3. Generate request/response sequences with parameters (timeout values, request types, response delays)
4. Set timeout threshold T (e.g., 5 seconds)
5. Send request, delay response beyond T (or drop response)
6. Verify timeout triggers (within T + margin)
7. Verify cleanup: connection closed, resources freed, error returned
8. Verify recovery: system can retry or handle error gracefully

## Checks

Verify timeout handling:

- **Timeout detection**: Send request, delay response > timeout T → assert timeout triggers within T + margin (e.g., T + 10%). Check timeout mechanism (timer expires, error returned)
- **Connection handling**: After connection timeout, verify: (1) connection closed, (2) resources freed (file descriptors, memory), (3) error returned to caller
- **Recovery**: After timeout, verify: (1) system can retry operation (if applicable), (2) system handles error gracefully (doesn't crash), (3) system state consistent (no leaked resources)
- **Correct behavior**: Verify timeout behavior matches specification: (1) retry: operation retried after timeout, (2) fail: error returned, (3) cleanup: resources freed

**Concrete checks**:
1. Send request, delay response > timeout T
2. Measure time until timeout → assert timeout triggers within T + margin
3. After timeout: check connection state → assert closed
4. After timeout: check resources → assert freed (no leaks)
5. After timeout: check error → assert error returned (e.g., timeout error)
6. Verify recovery: retry operation (if applicable) → assert succeeds
7. Verify state: system state consistent (no corruption, no leaks)

## Subdomains

**Initial state: connection established with keepalive**: Connection starts established with keepalive enabled. Action sequence: Connection idle for extended period, keepalive timeout expires. Test: verify connection timeout with keepalive (connection closed gracefully), resources freed, no leaks.

**Initial state: protocol with retry enabled**: Protocol starts with retry enabled. Action sequence: Operation times out, protocol retries. Test: verify operation timeout with retry (retry doesn't cause duplicate operations), timeout handled correctly.

**Initial state: protocol receiving partial response**: Protocol starts receiving partial response. Action sequence: Response partially received, then timeout occurs. Test: verify timeout during partial response (partial response handled: discarded or buffered), timeout triggers correctly.

**Initial state: protocol with dependent operations**: Protocol starts with operations that depend on each other. Action sequence: One timeout causes another operation to timeout (cascade effect). Test: verify cascading timeouts (system handles cascading timeouts), doesn't deadlock, recovers gracefully.

**Initial state: protocol with network partition**: Protocol starts with network partition active. Action sequence: Network partition causes timeouts (messages delayed/dropped). Test: verify timeout with network partition (timeouts trigger correctly), system handles partition, recovers when partition heals.

**Initial state: protocol with adaptive timeouts**: Protocol starts with adaptive timeout mechanism (RTT estimation). Action sequence: Network conditions vary, RTT changes. Test: verify adaptive timeout adjustment (timeout adapts to network conditions), not too short (false timeouts) or too long (slow recovery).
