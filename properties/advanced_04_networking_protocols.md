# Networking / Protocols Properties

**Property**: Messages delivered reliably and in order. System handles network conditions (congestion, timeouts) correctly.

## Concept

Generate message sequences and simulate network conditions. Verify:
- Messages delivered reliably (at-least-once, at-most-once, exactly-once)
- Messages delivered in order (FIFO, causal, total)
- System adapts to network conditions (congestion control)
- Timeouts handled correctly

**Key idea**: Network protocols should deliver messages reliably and in order despite network conditions.

## Actions

Focus on message delivery and network conditions:

- **Message sending**: Send messages through network
- **Network conditions**: Simulate congestion, delays, packet loss
- **Timeout scenarios**: Simulate timeouts, connection failures
- **Ordering scenarios**: Messages that should be delivered in order

**Network simulation**: Simulate network conditions (latency, packet loss, congestion), vary message ordering.

## Checks

Verify protocol properties:

- **Reliability**: Messages eventually delivered (according to guarantee: at-least-once, at-most-once, exactly-once)
- **Ordering**: Messages delivered in correct order (FIFO, causal, total)
- **Congestion handling**: System adapts to network conditions
- **Timeout handling**: Timeouts handled correctly, connections recovered

**Message tracking**: Track message delivery, verify ordering, check for duplicates or losses.

## Subdomains

**Normal network**: No packet loss, normal latency. Baseline behavior.

**Network conditions**: Packet loss, delays, congestion, timeouts that should be handled.

**Network stress**: High packet loss, extreme delays, congestion scenarios, many timeouts.
