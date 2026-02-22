# Property Derivation Framework

Methodology for deriving property-based tests from various sources of truth.

## Motivation

**Meta-testing goal**: Make cppproptest2 itself fully property-based tested. If we cannot test our own PBT framework with properties, how can we claim PBT is superior?

**Core principle**: Properties serve as **checkable documentation** of code behavior, more valuable than isolated test cases.

## Sources for Deriving Properties

### 1. Software Design

- **Invariants**: Class/data structure/state invariants
- **Pre/post conditions**: Expected outcomes before/after execution
- **Design contracts**: `requires` clauses, assertions

### 2. Documentation

- **API docs**: Extract stated behaviors ("returns sorted", "throws on negative")
- **README/guides**: Performance guarantees, usage examples, limitations

### 3. Specification

- **Formal specs**: Convert predicates to executable properties
- **Informal specs**: Requirements, user stories → properties
- **Standards**: RFCs, protocols → compliance properties

### 4. Comments

- **Inline docs**: `// NOTE:`, `// INVARIANT:` → properties
- **Design notes**: "This ensures X" → `ensuresX(...)`

### 5. General Sources of Truth

- **Mathematical knowledge**: Commutativity, idempotency, associativity
- **Industry conventions**: Standard library behavior, common patterns
- **Domain expertise**: Business rules, constraints, user expectations

## Property Derivation Patterns

1. **Invariants**: Properties that remain true throughout execution
2. **Postconditions**: Expected outcomes after function execution
3. **Metamorphic properties**: Relationships between test cases (e.g., `reverse(reverse(x)) == x`)
4. **Equivalence preservation**: Equivalent inputs → equivalent outputs (e.g., same seed → same sequence)
5. **Inductive properties**: Hold recursively/iteratively
6. **Model-based**: Compare against reference implementation

## The Quantifier View of Properties

Requirements can be read as logical statements with **quantifiers**: "for all," "for some," "for any," "for no." Making this explicit improves property identification and test design.

### Quantifiers

- **∀ (for all)** — "For all x in domain D, P(x)." Example: for all inputs, the output is correct.
- **∃ (for some)** — "There exists x in D such that P(x)." Example: there exists a recovery path after failure.
- **For any** — Same as ∀; emphasizes that the choice of x is arbitrary (e.g. any order of operations).
- **¬∃ (for no)** — "There does not exist x such that bad(x)." Example: for no execution does the system crash (safety).

Defining the **domain D** explicitly (e.g. "all operation sequences," "all interleavings," "all failure patterns") surfaces missing test dimensions.

### Implications

#### 1. Stating and scoping the property

**1.1. Explicit domain D**

Writing "for all x in D, P(x)" forces you to define D. Often we only test a **subset** of D (e.g. single-threaded sequences). Making D explicit **surfaces missing dimensions** (concurrency, failures, reordering) and suggests new generators or dimensions to add.

Separate **domain of the requirement** (what we claim) from **domain of generation** (what we test; can be a superset to include stress/negative cases).

#### 2. Structuring the test

**2.1. Nested quantifiers map to test structure**

Requirements often have multiple quantifiers, e.g. "For all clients and all requests, there exists a response within time T" → ∀ clients ∀ requests ∃(response by T). This maps to:

- **Outer ∀** → generate (client, request).
- **Inner ∃** → for each run, check that a response occurs by T (e.g. with a timeout).

The logical structure maps to **layers of generation and assertion**: what is universally quantified (generators) vs what is existentially quantified (something we observe or search for in a single run).

**2.2. Where to assert**

For "for all initial states, for all operation sequences, no intermediate state violates the invariant" (∀ ∀ ¬∃), assert at every step (or every observable state).

**2.3. Safety vs liveness**

- **Safety**: "No execution reaches a bad state" → ¬∃(execution : bad state). Test by trying to find a counterexample.
- **Liveness**: "Every execution eventually satisfies good" → ∀ execution ∃(finite prefix : good). Test by running with a bound (e.g. timeout) and asserting "good" within that bound.

#### 3. Discovering properties and designing generators

**3.1. Generator design follows the quantifier**

- **∀**: Sample the domain with good coverage.
- **¬∃ (safety)**: Same as ∀ over "not bad," but frame tests as **hunting for a counterexample**. Bias generators toward likely-bad cases (stress sequences, malformed input, adversarial interleavings) so violations are found faster.
- **∃**: Need **search for a witness** (e.g. random exploration, bounded search), not just "generate and check."

**3.2. "For some" is underused**

Many PBT examples are universal (∀ or ¬∃). **Existential** properties are less common but important:

- "There exists a recovery path after failure."
- "There exists a schedule where all threads make progress (no deadlock)."
- "There exists an input that reaches this branch."

For these, the "test" is **search** (random exploration, model checking, or symbolic execution). Design ∃-tests explicitly: witness search, coverage of progress or reachability.

---

**Summary**: State the requirement in ∀/∃/¬∃ form, define D, then choose generator strategy accordingly.

## Stateful Components: The Real Challenge

Stateful components (objects with mutable state, state machines, systems with side effects) are **far more complex** than stateless ones and where **sophisticated subdomain exploration is crucial**.

**Challenges**:
- **State space explosion**: Sequences of actions create exponential state spaces
- **Invariant identification**: What invariants hold across all state transitions?
- **Model complexity**: Need reference model (simplified version) to compare against
- **Action sequences**: Must generate valid action sequences, not just inputs
- **Post-state validation**: After action sequence, verify final state matches model

**Why subdomain exploration matters**:
- **Action sequence subdomains**: Short sequences vs long, valid vs invalid, common patterns vs edge cases
- **State-dependent actions**: Some actions only valid in certain states (e.g., `pop_back` on empty vector)
- **Weighted probabilities critical**: Boost probability of interesting action combinations, state transitions, error paths
- **Hyperparameters essential**: Randomize action weights, sequence lengths, state-dependent probabilities to explore different exploration strategies

**Deriving stateful properties**:
- **State invariants**: Properties that hold in all states (e.g., "size ≥ 0", "no cycles")
- **Action postconditions**: After each action, verify state change matches model
- **Sequence properties**: Valid sequences preserve invariants; invalid sequences handled correctly
- **Model-based**: Compare implementation state against simplified model after each action

**Example approach**:
1. Identify state invariants from design/docs
2. Create simplified model (e.g., `VectorModel` with just `size` for `vector<int>`)
3. Generate action sequences with **weighted probabilities**:
   - Boost error-path actions (e.g., `pop_back` on empty, `clear` after operations)
   - Weight by state-dependency (actions valid only in certain states)
   - Vary sequence lengths (short for quick checks, long for stress)
4. After each action: verify state matches model, invariants hold
5. After sequence: verify final state matches model

**Key insight**: Stateful testing requires both **action sequence generation** and **state model comparison**—much harder than pure function properties. **Sophisticated subdomain exploration** (weighted probabilities, hyperparameters) is essential to effectively explore the exponential state space.

### Fundamental Reusable Properties for Stateful Testing

While stateful testing is complex, there are **fundamental reusable properties** that apply across many stateful components:

**1. Algorithms / Data Structures**
- Algorithm produces correct output, meets complexity bounds, maintains data structure invariants
- Property: Correctness, time/space complexity within stated bounds, data structure invariants maintained
- Test: Run algorithm on generated inputs, verify correctness and complexity; verify data structure invariants
- **Subdomain**: Normal inputs, edge cases, complexity stress tests

**2. Reference Comparison / Refactoring**
- New implementation produces same output as reference for all inputs; meets performance criteria
- Property: Correctness by comparison, performance comparable to reference, complexity requirements met
- Test: Use existing code as oracle; generate inputs, compare outputs and performance
- **Subdomain**: Edge cases, error paths that might break during refactoring/enhancement

**3. Consistency / Invariants**
- Internal invariants maintained across all operations
- Property: Component-specific invariants hold in all states (e.g., "size ≥ 0", "no cycles", "sorted")
- Test: Verify invariants after each action, across all state transitions
- **Subdomain**: Focus on operations that might violate invariants

**4. Observability / State queries**
- State queries return correct values
- Property: Query methods (size, empty, contains, etc.) return values consistent with actual state
- Test: After each action, verify queries match model state
- **Subdomain**: Query at different points in sequence, after errors, after exceptions

**5. Resource management**
- Resources (handles, connections, locks) acquired are released
- Property: RAII compliance, no resource leaks, cleanup happens correctly
- Test: Track resource acquisition/release, verify all acquired resources are released
- Applies to: File handles, network connections, database connections, locks

**6. Exception-safety**
- If component requires exception-safety guarantees (basic, strong, nothrow)
- Property: Exceptions don't leave component in invalid state
- **Levels**: Basic guarantee (valid state), strong guarantee (rollback), nothrow guarantee (no exceptions)
- Test: Inject exceptions at various points, verify state invariants hold

**7. Memory safety**
- No memory leaks, double-free, use-after-free, buffer overflows
- Property: After action sequences and destruction, no memory leaks; no invalid memory access
- Test: Use allocation tracker, verify allocations match deallocations; use sanitizers (ASAN, MSAN)
- **Subdomain**: Vary sequence lengths, stress test with many operations

**8. Thread safety / Concurrency safety**
- Safe concurrent access from multiple threads
- Property: Concurrent operations maintain invariants, no data races, no deadlocks
- Test: Generate concurrent action sequences, verify consistency after parallel execution
- **Subdomain**: Vary thread counts, operation mixes, timing (via delays)

**9. Malfunction tolerance**
- Component handles erroneous responses from external dependencies gracefully
- **Sources of malfunctions**:
  - Allocator failures (OOM, allocation errors)
  - External components (network failures, I/O errors, system call failures)
  - Probabilistic malfunction injection: control probability of failures
- Property: Component maintains invariants, handles errors correctly, doesn't corrupt state
- Test: Inject malfunctions with weighted probabilities, verify graceful handling

**10. Crash tolerance**
- Component doesn't crash/abort on any action sequence
- Property: After any sequence of actions, component is in valid state (no crashes, no undefined behavior)
- Test: Generate random action sequences, verify no crashes occur

**Why these are reusable**:
- Apply to many stateful components (containers, resource managers, state machines)
- Can be parameterized by component type and action set
- Use weighted probabilities to control malfunction/exception injection rates
- Hyperparameters: randomize malfunction probabilities to explore different failure scenarios

**Example**: Test a `vector`-like container:
- **Crash tolerance**: Random `push_back`/`pop_back`/`clear` sequences → no crashes
- **Malfunction tolerance**: Inject allocator failures → verify graceful handling, no corruption
- **Exception-safety**: Inject exceptions in copy/move operations → verify strong guarantee (rollback on exception)
- **Memory safety**: Track allocations → verify no leaks after destruction
- **Thread safety**: Concurrent `push_back`/`pop_back` → verify no races, consistent size
- **Observability**: After each operation, verify `size()`, `empty()`, `at()` match actual state
- **Consistency**: Invariant "size ≥ 0" holds after all operations

### Advanced Domain-Specific Properties

Beyond fundamental properties, there are **advanced reusable properties** based on concepts from various fields of computer science:

### Fundamental Advanced Properties

These apply broadly across many systems and domains:

**Concurrency / Parallel Computing**:
- **Serializability**: Concurrent execution equivalent to some serial execution
- **Liveness**: System makes progress (no deadlocks, starvation, livelocks)
- **Safety**: Nothing bad happens (no data races, no corruption)
- **Lock ordering**: Consistent lock acquisition order prevents deadlocks
- **Wait-freedom**: Operations complete in finite steps regardless of other threads

**Database / Transactional Properties (ACID)**:
- **Atomicity**: Transactions are all-or-nothing (all operations commit or all rollback)
- **Consistency**: Transactions preserve database invariants
- **Isolation**: Concurrent transactions don't interfere (visibility rules: uncommitted changes only visible to current transaction)
- **Durability**: Committed transactions persist despite failures
- **Test**: Generate transaction sequences, inject failures, verify ACID properties hold

### Domain-Specific / Niche Properties

These apply to specific domains or systems:

**CAP Theorem Properties** (Distributed Systems):
- **Consistency**: All nodes see same data simultaneously
- **Availability**: System remains operational
- **Partition tolerance**: System continues despite network partitions
- **Test**: Simulate network partitions, verify chosen CAP tradeoffs hold

**Distributed Systems Properties**:
- **Eventual consistency**: System converges to consistent state
- **Causal consistency**: Causally related events seen in order
- **Linearizability**: Operations appear atomic and in some total order
- **Test**: Generate distributed operation sequences, verify consistency model

**Cache / Memory Systems**:
- **Cache coherence**: All caches see consistent view of memory
- **Cache eviction**: LRU/FIFO/etc. policies work correctly
- **Write-through/write-back**: Writes handled according to policy
- **Memory ordering**: Sequential consistency, release-acquire semantics

**Networking / Protocols**:
- **Reliability**: Messages eventually delivered (at-least-once, at-most-once, exactly-once)
- **Ordering**: Messages delivered in order (FIFO, causal, total)
- **Congestion control**: System adapts to network conditions
- **Fairness**: Bandwidth/resources shared fairly among flows
- **Timeout handling**: Connections/timeouts handled correctly

**Security**:
- **Authentication**: Only authorized users can access
- **Authorization**: Users can only access permitted resources
- **Confidentiality**: Sensitive data not leaked
- **Integrity**: Data not tampered with
- **Non-repudiation**: Actions cannot be denied
- **Forward secrecy**: Past communications remain secure if key compromised

**Time / Event Systems**:
- **Causality**: Causally related events seen in order
- **Monotonicity**: Timestamps/versions only increase
- **Clock synchronization**: Clocks synchronized within bounds
- **Event ordering**: Events processed in correct order

**Compilers / Language Systems**:
- **Type safety**: Type errors caught, no invalid operations
- **Optimization correctness**: Optimizations preserve semantics
- **Memory safety**: No buffer overflows, use-after-free
- **Soundness**: Type system sound (well-typed programs don't go wrong)

**Operating Systems**:
- **Fairness**: CPU/resources shared fairly among processes
- **Isolation**: Processes don't interfere with each other
- **Resource limits**: Processes respect memory/CPU limits
- **Priority scheduling**: Higher priority processes scheduled first

**Load Balancing**:
- **Fairness**: Load distributed evenly across servers
- **Sticky sessions**: Same client routed to same server (if required)
- **Health checks**: Unhealthy servers removed from pool
- **Consistent hashing**: Minimal redistribution when servers added/removed

**Cryptography**:
- **Semantic security**: Encrypted messages indistinguishable from random
- **Authenticity**: Messages authenticated (MAC, signatures)
- **Key derivation**: Keys derived correctly from secrets
- **Randomness**: Cryptographic randomness properties

**Game Theory / Fairness**:
- **Nash equilibrium**: Strategy profiles stable
- **Fairness**: Resources allocated fairly (proportional, max-min)
- **Strategy-proofness**: No incentive to misreport preferences

**Why these matter**:
- **Fundamental properties**: Apply broadly across many systems (databases, distributed systems, concurrent systems). Well-established CS theory → directly map to testable properties. Reusable across implementations of same concept (e.g., all databases should satisfy ACID).
- **Domain-specific properties**: Apply to specific domains (networking, compilers, OS, etc.). Domain experts already understand these concepts → easier to derive properties. Useful when testing components in those specific domains.

## Subdomain Exploration

See [Subdomain Exploration](subdomainexploration.md) for:
- Discrete subdomains vs weighted probabilities
- Hyperparameters: randomized probability distributions

## Practical Guidelines

1. **Audit existing tests**: Identify what they verify → extract property → determine subdomains
2. **Document sources**: Always note where properties come from
3. **Identify subdomains**: Edge cases, error paths, common patterns, rare-but-important cases
4. **Use weighted generators**: Prefer over discrete subdomains for realistic testing
5. **Explore hyperparameters**: For critical properties, randomize probability distributions
6. **Use reference implementations**: During refactoring, compare against existing code

## Example: Converting Example-Based Test

**Before**:
```cpp
TEST(Property, forAll) {
    EXPECT_TRUE(forAll([](int a, int b) {
        return a + b == b + a;
    }, gen::interval(0, 10), gen::interval(0, 10)));
}
```

**After**:
```cpp
// Property: commutativity of addition (mathematical property)
// Subdomains: zero, extremes, normal range
forAll([](int a, int b) {
    PROP_EXPECT_EQ(a + b, b + a);
    return true;
}, gen::oneOf(
    gen::weighted(0.1, gen::just(0)),
    gen::weighted(0.1, gen::just(INT_MAX)),
    gen::weighted(0.1, gen::just(INT_MIN)),
    gen::weighted(0.7, gen::interval(-1000, 1000))
), gen::oneOf(
    gen::weighted(0.1, gen::just(0)),
    gen::weighted(0.1, gen::just(INT_MAX)),
    gen::weighted(0.1, gen::just(INT_MIN)),
    gen::weighted(0.7, gen::interval(-1000, 1000))
));
```

## Framework Methodology

1. **Documentation**: Create guidelines, patterns, examples
2. **Audit & convert**: Core tests → properties with subdomain exploration
3. **Expand**: All components, maintain derivation source docs
4. **Showcase**: Side-by-side comparisons, demonstrate effectiveness

## Conclusion

Systematically derive properties from design, documentation, specs, comments, and general sources. Use subdomain exploration (weighted probabilities, hyperparameters) to make tests effective at finding issues. **Stateful components are the real challenge**—they require action sequence generation, state model comparison, and invariant identification across state transitions. Properties are checkable documentation, more valuable than isolated test cases.
