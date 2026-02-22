# Liveness Property

**Property**: System makes progress. No deadlocks, starvation, or livelocks occur.

## Concept

Test that concurrent system maintains liveness: system makes progress, threads don't get stuck. Verify:
- No deadlocks occur
- No starvation (threads eventually get resources)
- No livelocks (threads make progress)
- System continues to make progress

**Key idea**: System should make progress; threads should eventually complete their operations.

## Actions

Focus on progress scenarios:

- **Initial state generation**: Generate initial system state (no locks held, some locks held, resources available/unavailable, data structures in various states). Initial state determines what deadlock/starvation scenarios are possible.
- **Serial actions (setup)**: Set up conditions that create potential deadlock/starvation/livelock scenarios (e.g., thread1 acquires lock A, thread2 acquires lock B, leave both holding locks). Serial actions prepare the state space for concurrent testing.
- **Parallel actions (concurrent execution)**: Start operations concurrently that compete for locks/resources (e.g., thread1 tries to acquire lock B, thread2 tries to acquire lock A). Parallel actions test liveness guarantees.
- **Lock acquisition patterns**: Generate operations that acquire multiple locks in different orders (e.g., thread1: lock A then B, thread2: lock B then A) → potential deadlock
- **Resource contention**: Generate operations competing for limited resources (e.g., N threads, M resources where M < N) → potential starvation
- **Retry loops**: Generate operations that retry on failure (e.g., CAS operations that keep failing) → potential livelock

**Concrete plan**:
1. Generate initial system state (locks, resources, data structures)
2. Serial actions: Set up conditions (acquire locks, allocate resources, prepare data structures)
3. Parallel actions: Start concurrent operations that compete (acquire locks, request resources, retry operations)
4. Monitor progress: track which operations complete, which are blocked
5. Use timeout: if operation doesn't complete within T seconds, potential deadlock/starvation
6. Check state changes: if operation retries but state doesn't change, potential livelock
7. Verify all operations complete within timeout

Verify liveness:

- **Bounded completion**: Set timeout T (e.g., 10 seconds). Start all operations, wait up to T → assert all operations complete within T. If timeout expires, potential deadlock
- **No starvation**: For N threads competing for M resources (M < N), verify all threads eventually get resources. Track resource acquisition times → assert no thread waits indefinitely (within timeout T)
- **No livelocks**: For operations that retry, track state changes. If operation retries K times but state doesn't change → potential livelock. Assert: state changes occur, or operation completes
- **Progress**: Monitor system state over time (every 100ms). Assert: state changes occur (operations making progress), or all operations complete

**Concrete checks**:
1. Start all operations, set timeout T
2. Monitor: every 100ms, check which operations completed
3. Within T: assert all operations completed
4. For starvation: track resource acquisition → assert all threads acquired resources within T
5. For livelock: track retry count and state → if retries > threshold and state unchanged → violation
6. Assert: no deadlocks (all complete), no starvation (all get resources), no livelocks (state changes)

## Subdomains

**Initial state: no locks held**: System starts with no locks held. Serial actions: Thread1 acquires lock A, Thread2 acquires lock B. Parallel actions: Thread1 tries lock B, Thread2 tries lock A. Test: verify circular wait deadlock detected or prevented.

**Initial state: some locks already held**: System starts with locks held by other threads. Serial actions: Thread1 acquires lock A (waits for existing holder). Parallel actions: Thread2 acquires lock B, then tries lock A. Test: verify deadlock scenarios with existing lock holders.

**Initial state: resources partially allocated**: System starts with some resources allocated. Serial actions: Allocate more resources (approaching limit). Parallel actions: Many threads request remaining resources. Test: verify resource starvation scenarios, all threads eventually get resources.

**Initial state: data structure in specific state**: System starts with data structure in state that causes retries (e.g., full, empty, at capacity). Serial actions: Set up conditions for retry loops. Parallel actions: Multiple threads retry operations. Test: verify livelock scenarios, operations eventually succeed.

**Initial state: priority-based system**: System starts with threads at different priorities. Serial actions: Low-priority thread acquires lock. Parallel actions: High-priority thread tries same lock, medium-priority thread preempts. Test: verify priority inversion scenarios, priority inheritance prevents inversion.

**Initial state: high contention state**: System starts with many threads waiting. Serial actions: Create convoy conditions (many threads acquire/release same lock). Parallel actions: More threads join convoy. Test: verify lock convoy scenarios, system makes progress despite convoy.
