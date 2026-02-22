# Database / Transactional Properties (ACID)

Strategies for testing ACID properties in database/transactional systems.

## Properties

1. **[Atomicity](01_atomicity.md)** - All operations in transaction commit or all rollback
2. **[Consistency](02_consistency.md)** - Transactions preserve database invariants
3. **[Isolation](03_isolation.md)** - Concurrent transactions don't interfere
4. **[Durability](04_durability.md)** - Committed transactions persist despite failures

## Overview

ACID properties ensure transactional correctness. Each property requires specific testing strategies:
- **Atomicity**: Test partial failures, verify all-or-nothing behavior
- **Consistency**: Test invariant preservation across transactions
- **Isolation**: Test concurrent access patterns, verify visibility rules
- **Durability**: Test persistence after failures, verify recovery
