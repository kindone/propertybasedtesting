# Property Test Derivation Checklist

A systematic checklist for deriving property-based test candidates from a given component.

## Component Analysis

- [ ] **Identify component type**: Stateless function, stateful object, state machine, distributed system, etc.
- [ ] **Identify inputs**: What are the input parameters/types?
- [ ] **Identify outputs**: What does the component return/produce?
- [ ] **Identify state**: If stateful, what state does it maintain?
- [ ] **Identify operations**: What operations/actions can be performed?
- [ ] **Identify dependencies**: External components, allocators, system calls, etc.

## Source Analysis

### Software Design
- [ ] **Invariants**: What invariants are stated or implied? (class invariants, data structure invariants, state invariants)
- [ ] **Preconditions**: What preconditions are documented or enforced?
- [ ] **Postconditions**: What postconditions are guaranteed?
- [ ] **Design contracts**: `requires` clauses, assertions, contracts in code

### Documentation
- [ ] **API documentation**: Extract stated behaviors ("returns sorted", "throws on negative", "thread-safe")
- [ ] **README/guides**: Performance guarantees, usage examples, known limitations
- [ ] **Comments**: `// NOTE:`, `// INVARIANT:`, `// TODO:` items

### Specification
- [ ] **Formal specs**: Mathematical or formal language specifications
- [ ] **Informal specs**: Requirements documents, user stories
- [ ] **Standards**: Industry standards, RFCs, protocols (if applicable)

### General Sources of Truth
- [ ] **Mathematical knowledge**: Commutativity, idempotency, associativity, domain-specific invariants
- [ ] **Industry conventions**: Standard library behavior, common patterns
- [ ] **Domain expertise**: Business rules, constraints, user expectations

### Reference Implementation (for refactoring)
- [ ] **Existing code**: Is there existing code that can serve as reference?
- [ ] **Correctness comparison**: Can compare outputs?
- [ ] **Performance comparison**: Can compare time/space complexity?

## Property Derivation Patterns

### For Stateless Components
- [ ] **Invariants**: Properties that remain true (if applicable)
- [ ] **Postconditions**: Expected outcomes after function execution
- [ ] **Metamorphic properties**: Relationships between test cases (e.g., `reverse(reverse(x)) == x`)
- [ ] **Equivalence preservation**: Equivalent inputs → equivalent outputs
- [ ] **Inductive properties**: Hold recursively/iteratively
- [ ] **Model-based**: Compare against reference implementation

### For Stateful Components
- [ ] **State invariants**: Properties that hold in all states
- [ ] **Action postconditions**: After each action, verify state change matches model
- [ ] **Sequence properties**: Valid sequences preserve invariants; invalid sequences handled correctly
- [ ] **Model-based**: Compare implementation state against simplified model

## Fundamental Properties (Stateful Components)

- [ ] **Crash tolerance**: Component doesn't crash on any action sequence
- [ ] **Malfunction tolerance**: Handles erroneous responses from allocators/external components gracefully
- [ ] **Exception-safety**: Exceptions don't leave component in invalid state (basic/strong/nothrow)
- [ ] **Memory safety**: No leaks, double-free, use-after-free
- [ ] **Thread safety**: Safe concurrent access (if applicable)
- [ ] **Resource management**: Resources acquired are released (RAII compliance)
- [ ] **Observability**: State queries return correct values
- [ ] **Consistency**: Internal invariants maintained across all operations

## Advanced Properties (if applicable)

### Fundamental Advanced Properties
- [ ] **Database/Transactional (ACID)**: Atomicity, Consistency, Isolation, Durability
- [ ] **CAP Theorem**: Consistency, Availability, Partition tolerance
- [ ] **Distributed Systems**: Eventual consistency, causal consistency, linearizability
- [ ] **Concurrency**: Serializability, liveness, safety, lock ordering, wait-freedom
- [ ] **Algorithms/Data Structures**: Correctness, complexity bounds, invariants, stability
- [ ] **Reference Implementation**: Correctness by comparison, performance criteria

### Domain-Specific Properties (if applicable)
- [ ] **Cache/Memory Systems**: Cache coherence, eviction policies, memory ordering
- [ ] **Networking/Protocols**: Reliability, ordering, congestion control, fairness
- [ ] **Security**: Authentication, authorization, confidentiality, integrity
- [ ] **Time/Event Systems**: Causality, monotonicity, clock synchronization
- [ ] **Other domain-specific**: Compilers, OS, load balancing, cryptography, etc.

## Subdomain Identification

- [ ] **Edge cases**: Boundaries, empty inputs, extreme values (0, INT_MAX, INT_MIN, nullptr, empty containers)
- [ ] **Error paths**: Invalid inputs, exception cases, out-of-range indices
- [ ] **Common patterns**: Typical usage scenarios, normal-sized inputs
- [ ] **Rare but important**: Corner cases that matter, specific values triggering special logic
- [ ] **State-dependent**: For stateful components, actions only valid in certain states
- [ ] **Action sequences**: For stateful components, short vs long sequences, valid vs invalid

## Generator Design

- [ ] **Input generators**: Design generators for each input type
- [ ] **Weighted probabilities**: Use weighted generators to boost corner cases
  - [ ] Identify which subdomains need higher weights
  - [ ] Assign weights (e.g., 10-30% for edge cases, 40-70% for normal cases)
- [ ] **Hyperparameters**: Consider randomizing probability distributions for critical properties
- [ ] **State-dependent generators**: For stateful components, generators that respect state constraints
- [ ] **Action sequence generators**: For stateful components, generators for valid action sequences

## Property Test Implementation

- [ ] **Write property**: Express property as executable code
- [ ] **Choose generators**: Select or create appropriate generators
- [ ] **Apply subdomain exploration**: Use weighted probabilities or hyperparameters
- [ ] **Document derivation source**: Note where property came from (design doc, comment, etc.)
- [ ] **Test property**: Run property test, verify it finds issues or validates behavior

## Review and Refinement

- [ ] **Property coverage**: Do properties cover all important behaviors?
- [ ] **Subdomain coverage**: Are meaningful subdomains explored?
- [ ] **Bug-finding effectiveness**: Do properties find real bugs or validate correctness?
- [ ] **Performance**: Are tests fast enough? Can complexity be reduced?
- [ ] **Maintainability**: Are properties clear and maintainable?

## Example Workflow

1. **Analyze component**: Identify type, inputs, outputs, state, operations
2. **Search sources**: Check design docs, API docs, comments, specs
3. **Derive properties**: Apply property derivation patterns
4. **Identify subdomains**: List edge cases, error paths, common patterns
5. **Design generators**: Create generators with weighted probabilities
6. **Implement tests**: Write property tests with proper generators
7. **Review**: Check coverage, effectiveness, maintainability

## Notes

- **Start simple**: Begin with basic properties (correctness, invariants), then add advanced properties
- **Iterate**: Properties can be refined as understanding of component grows
- **Document**: Always document where properties come from for future reference
- **Subdomain exploration is crucial**: Especially for stateful components with exponential state spaces
- **Reference implementations**: Use existing code as oracle during refactoring/enhancements
