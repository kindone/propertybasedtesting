# Reliability Property

**Property**: Messages eventually delivered according to guarantee (at-least-once, at-most-once, exactly-once).

## Concept

Test that protocol maintains reliability guarantee: messages are delivered according to specified guarantee. Verify:
- At-least-once: Messages delivered at least once (may duplicate)
- At-most-once: Messages delivered at most once (may lose)
- Exactly-once: Messages delivered exactly once (no duplicates, no losses)
- Delivery eventually happens

**Key idea**: Protocol should deliver messages according to its reliability guarantee despite network conditions.

## Actions

Focus on message delivery scenarios:

- **Initial state generation**: Generate initial protocol state (no pending messages, some messages in flight, retransmissions pending, connections established). Initial state determines what reliability scenarios are possible.
- **Action sequences with parameters**: Generate message sending sequences with specific parameters (message IDs, sizes, priorities, retry policies). Action sequences test reliability guarantees across different network conditions.
- **Message sending**: Generate messages with unique IDs. Send messages through mock network layer. Track sent messages (message ID, timestamp)
- **Simulated network conditions**: Implement mock network layer: (1) packet loss: drop messages with probability P (e.g., 10%), (2) delays: delay messages by random time T (e.g., 0-100ms), (3) reordering: reorder messages randomly
- **Delivery scenarios**: Test each guarantee: (1) at-least-once: message may be delivered multiple times, (2) at-most-once: message delivered at most once (may be lost), (3) exactly-once: message delivered exactly once
- **Duplicate scenarios**: Simulate retransmissions (sender retries after timeout), network reordering (messages arrive out of order) → may cause duplicates

**Concrete plan**:
1. Generate initial protocol state (no pending messages, connections established, retransmissions pending)
2. Implement mock network layer with loss/delay/reordering
3. Generate message sending sequences with parameters (N messages with unique IDs, sizes, priorities)
4. Send messages, track sent messages (ID, timestamp)
5. Receive messages, track received messages (ID, timestamp, count)
6. Compare sent vs received: check for losses, duplicates
7. Verify delivery guarantee matches specification

Verify reliability:

- **Delivery guarantee**: Compare sent messages (set S) vs received messages (multiset R). Check:
  - At-least-once: for each message in S, assert it appears at least once in R (may appear multiple times)
  - At-most-once: for each message in R, assert it appears at most once (but some messages in S may not appear in R)
  - Exactly-once: assert S == R as sets (each message in S appears exactly once in R, no extras)
- **No loss**: For at-least-once/exactly-once: count messages in S vs R → assert |R| >= |S| (all messages delivered)
- **No duplicates**: For at-most-once/exactly-once: check for duplicate IDs in R → assert no duplicates (each message ID appears at most once)
- **Eventual delivery**: Wait for delivery period (with timeout T). After T, check delivery → assert all messages delivered (for at-least-once/exactly-once)

**Concrete checks**:
1. Track sent messages: set S = {message IDs}
2. Track received messages: multiset R = {message IDs with counts}
3. For at-least-once: assert S ⊆ R (all sent messages received, may have duplicates)
4. For at-most-once: assert R ⊆ S and no duplicates in R (received messages are subset of sent, no duplicates)
5. For exactly-once: assert S == R as sets and no duplicates (same set, no duplicates)
6. Count losses: |S| - |unique(R)| → assert losses match guarantee (zero for at-least-once/exactly-once)
7. Count duplicates: |R| - |unique(R)| → assert duplicates match guarantee (zero for at-most-once/exactly-once)

## Subdomains

**Initial state: protocol ready, no pending messages**: Protocol starts ready with no pending messages. Action sequence: Network drops many consecutive packets (burst loss). Test: verify burst packet loss (protocol recovers), messages eventually delivered despite burst.

**Initial state: multiple senders with pending retransmissions**: Protocol starts with multiple senders having pending retransmissions. Action sequence: Multiple senders retransmit simultaneously after timeout. Test: verify retransmission storms (protocol handles retransmissions), no duplicate storms.

**Initial state: protocol with messages in flight**: Protocol starts with messages already in flight. Action sequence: Network reorders messages AND duplicates some messages. Test: verify message reordering with duplicates (protocol handles both reordering and deduplication correctly).

**Initial state: protocol configured for at-least-once**: Protocol starts configured for at-least-once delivery. Action sequence: Messages may be delivered multiple times. Test: verify at-least-once with idempotency (receivers handle duplicates correctly), idempotent operations, no side effects from duplicates.

**Initial state: protocol configured for at-most-once**: Protocol starts configured for at-most-once delivery. Action sequence: Messages may be lost. Test: verify at-most-once with loss tolerance (protocol detects losses), applications handle missing messages gracefully.

**Initial state: protocol configured for exactly-once**: Protocol starts configured for exactly-once delivery. Action sequence: Messages sent despite network failures. Test: verify exactly-once delivery guarantees (deduplication works), no messages lost, no duplicates delivered.
