# Networking / Protocols Properties

Strategies for testing protocol properties in networked systems.

## Properties

1. **[Reliability](01_reliability.md)** - Messages eventually delivered (at-least-once, at-most-once, exactly-once)
2. **[Ordering](02_ordering.md)** - Messages delivered in order (FIFO, causal, total)
3. **[Congestion Control](03_congestion_control.md)** - System adapts to network conditions
4. **[Timeout Handling](04_timeout_handling.md)** - Connections/timeouts handled correctly

## Overview

Network protocols must maintain reliability and ordering guarantees:
- **Reliability**: Test message delivery guarantees
- **Ordering**: Test message ordering guarantees
- **Congestion control**: Test adaptation to network conditions
- **Timeout handling**: Test timeout and recovery behavior
