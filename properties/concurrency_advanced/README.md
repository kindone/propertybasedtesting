# Concurrency / Parallel Computing Properties

Strategies for testing advanced concurrency properties.

## Properties

1. **[Serializability](01_serializability.md)** - Concurrent execution equivalent to some serial execution
2. **[Liveness](02_liveness.md)** - System makes progress (no deadlocks, starvation, livelocks)
3. **[Safety](03_safety.md)** - Nothing bad happens (no data races, no corruption)
4. **[Wait-freedom](04_wait_freedom.md)** - Operations complete in finite steps regardless of other threads

## Overview

Advanced concurrency properties ensure correctness and progress in parallel systems:
- **Serializability**: Find equivalent serial execution
- **Liveness**: Verify progress (no deadlocks, starvation)
- **Safety**: Verify no bad things happen (no races, corruption)
- **Wait-freedom**: Verify operations complete independently
