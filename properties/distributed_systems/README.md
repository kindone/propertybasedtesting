# Distributed Systems Properties

Strategies for testing consistency models in distributed systems.

## Properties

1. **[Eventual Consistency](01_eventual_consistency.md)** - System converges to consistent state
2. **[Causal Consistency](02_causal_consistency.md)** - Causally related events seen in order
3. **[Linearizability](03_linearizability.md)** - Operations appear atomic and in some total order

## Overview

Distributed systems maintain different consistency models. Each requires specific testing strategies:
- **Eventual consistency**: Test convergence, verify all nodes eventually see same state
- **Causal consistency**: Test causal ordering, verify causally related events seen in order
- **Linearizability**: Test atomicity, verify operations appear in some total order
