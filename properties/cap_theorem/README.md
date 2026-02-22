# CAP Theorem Properties

Strategies for testing CAP theorem tradeoffs in distributed systems.

## Properties

1. **[Consistency](01_consistency.md)** - All nodes see same data simultaneously
2. **[Availability](02_availability.md)** - System remains operational
3. **[Partition Tolerance](03_partition_tolerance.md)** - System continues despite network partitions

## Overview

CAP theorem states that a distributed system can guarantee at most two of three properties:
- **Consistency**: All nodes see same data simultaneously
- **Availability**: System remains operational (responds to requests)
- **Partition tolerance**: System continues despite network partitions

Systems must choose which two to prioritize. Testing verifies the chosen tradeoffs hold.
