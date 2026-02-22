# Ordering Property

**Property**: Messages delivered in order according to guarantee (FIFO, causal, total).

## Concept

Test that protocol maintains ordering guarantee: messages are delivered in correct order. Verify:
- FIFO ordering: Messages from same sender delivered in order
- Causal ordering: Causally related messages delivered in order
- Total ordering: All messages delivered in same order to all receivers
- Ordering guarantee holds

**Key idea**: Protocol should deliver messages in order according to its ordering guarantee.

## Actions

Focus on message ordering scenarios:

- **Initial state generation**: Generate initial protocol state (no pending messages, connections established, sequence numbers initialized). Initial state determines what ordering scenarios are possible.
- **Action sequences with parameters**: Generate message sending sequences with specific parameters (sequence numbers, causal dependencies, sender IDs, priorities). Action sequences test ordering guarantees across different network conditions.
- **Ordered messages**: Generate messages from same sender with sequence numbers (1, 2, 3, ...). Send messages, verify delivery order matches send order
- **Causal messages**: Generate causal chain: message M1, message M2 that references M1 (causally depends on M1). Send both, verify M1 delivered before M2
- **Concurrent messages**: Generate messages from different senders (no causal dependency). These can be delivered in any order
- **Simulated network reordering**: Implement mock network layer: reorder messages randomly (shuffle delivery order). Test: ordering guarantee should prevent or correct reordering

**Concrete plan**:
1. Generate initial protocol state (no pending messages, connections established, sequence numbers initialized)
2. Implement mock network layer with reordering (shuffle messages)
3. Generate message sequences with ordering requirements and parameters:
   - FIFO: messages from same sender with sequence numbers
   - Causal: messages with causal dependencies (track dependencies)
   - Total: all messages with global sequence numbers
4. Send messages, record send order
5. Receive messages, record receive order
6. Verify ordering guarantee holds

Verify ordering:

- **FIFO ordering**: For messages from same sender S, compare send order (sequence numbers) vs receive order → assert receive order matches send order. If message M1 sent before M2, assert M1 received before M2
- **Causal ordering**: Track causal dependencies (M2 causally depends on M1). Check receive order → assert M1 received before M2 at all receivers
- **Total ordering**: Compare receive order across all receivers → assert all receivers see same order. If receiver R1 sees order [M1, M2, M3], receiver R2 should see same order
- **Ordering guarantee**: Check for violations: if ordering guarantee violated, assert protocol corrects it (buffers, reorders) or rejects out-of-order messages

**Concrete checks**:
1. Track send order: for each sender S, list messages in send order (by sequence number)
2. Track receive order: for each receiver R, list messages in receive order
3. For FIFO: for each sender S, compare send order vs receive order at each receiver → assert matches
4. For causal: for each causal dependency M1 → M2, check receive order at each receiver → assert M1 before M2
5. For total: compare receive orders across receivers → assert all identical
6. Count violations: if ordering violated, assert protocol handles it (buffers, rejects, or corrects)

## Subdomains

**Initial state: protocol ready, FIFO configured**: Protocol starts ready with FIFO ordering configured. Action sequence: Messages from same sender arrive out of order due to network. Test: verify FIFO with network reordering (protocol buffers and reorders), delivers in send order.

**Initial state: protocol with partition active**: Protocol starts with network partition active. Action sequence: Causally related messages sent across network partition. Test: verify causal ordering across partitions (causal order preserved), even when messages delayed/dropped, eventually delivered in order.

**Initial state: protocol with clock skew**: Protocol starts with nodes having clock skew. Action sequence: Messages sent requiring total ordering. Test: verify total ordering with clock skew (ordering based on logical timestamps or consensus), not wall-clock time.

**Initial state: protocol configured to reject out-of-order**: Protocol starts configured to reject out-of-order messages. Action sequence: Messages arrive out of order. Test: verify out-of-order message rejection (messages buffered until in-order), or rejected with proper error handling.

**Initial state: protocol with some messages lost**: Protocol starts with some messages already lost. Action sequence: More messages sent, some lost. Test: verify ordering with message loss (gaps handled correctly), ordering preserved for received messages.

**Initial state: protocol with multiple senders**: Protocol starts with multiple senders active. Action sequence: Messages from multiple senders need total ordering. Test: verify multi-sender ordering (global ordering maintained across all senders), all receivers see same order.
