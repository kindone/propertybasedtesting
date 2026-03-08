# General Guidelines for Property Design

**Property**: Prefer general, requirement-level properties over implementation-specific ones when designing and maintaining property tests.

## Concept

When designing properties, there is a tradeoff between:

- **General properties (spec-level)**: Express what the component must do from the caller’s point of view (e.g., map semantics, ordering guarantees, invariants that hold regardless of implementation).
- **Implementation-specific properties (internal details)**: Express how the component happens to be implemented (e.g., tree shape, bucket layout, particular balancing strategy).

General properties tend to be:
- **Stable** under refactoring (e.g., switching from a tree-based map to a hash-based map).
- **Easier to get right** because they are closer to the documented behavior and less entangled with internal complexity.
- **More reusable** across different implementations of the same abstraction.

Implementation-specific properties can:
- **Catch deep, subtle bugs** in a particular implementation.
- **Break easily** when internal data structures or algorithms are changed.
- **Be harder to write correctly**, because stating and checking detailed internal behavior is inherently more complex.

**Key idea**: By default, favor properties that talk about externally observable behavior and documented guarantees. Only test internal details when you deliberately want strong internal regression tests for a specific implementation.

## Actions

When defining actions and generators for a component:

- Focus on **externally meaningful operations** (API methods, observable state changes) rather than internal steps.
- Design actions so they make sense for any reasonable implementation of the abstraction (e.g., any `map` implementation, any `queue` implementation).
- If you introduce actions that are tightly coupled to internal structure (e.g., “inspect internal tree node layout”), clearly treat those as **implementation-only** tests.

## Checks

When writing checks:

- Ask: **“Would this check still make sense if I reimplemented this component with a different data structure?”**
  - If **yes** → it is a **good general property**.
  - If **no** → it is tied to **implementation details**; only keep it if you explicitly want to lock in that implementation.
- Prefer checks that:
  - Compare against a **model** or simpler reference implementation.
  - Assert **documented behavior** (e.g., ordering guarantees, idempotence, algebraic laws).
  - Assert **invariants** that any implementation must satisfy (e.g., size relationships, absence of duplicates).

Implementation-detail checks are still useful, but should be:
- Clearly labeled as **internal** or **implementation-specific**.
- Considered lower priority than general properties when maintaining the test suite.

### Overlaps and subsumption between properties

Over time, you will often find that some properties or checks are **subsumed** by others:

- If **B implies A** (every test that satisfies B necessarily satisfies A), you may not need to keep both.
- Example (radical): any **consistency** property that inspects the final state implicitly requires **no crash**; you cannot check consistency if the system crashes first.
- Example (practical): for an **encoder/decoder pair**, a strong inverse property  
  `decode(encode(x)) == x` over well-chosen subdomains can often replace many weaker checks on the internal structure of `encode(x)`.

Guidelines:

- Before removing a property **A** in favor of a more general **B**, ask:
  - Do we really believe B **covers** everything A was checking?
  - Are there edge cases that A exercised but B does not?
- Prefer **gradual changes**:
  - First add the more general property **B**.
  - Run both A and B for a while.
  - Only retire A after you gain confidence that B is stable and at least as effective.
- Be especially careful with \"hidden\" dimensions:
  - A consistency property might implicitly cover **no crash**, but a dedicated **crash tolerance** or **stress** property can still be valuable (e.g., high-load, long sequences, adversarial subdomains).

In short: look for opportunities to **merge or generalize** properties to keep the set small and meaningful, but remove older properties only when you are confident the new ones truly subsume them.

## Subdomains

When planning subdomains and coverage:

- Prioritize subdomains that exercise **general behavior**:
  - Different sizes and shapes of input (small, medium, large).
  - Edge cases at the API boundary (empty structures, maximum sizes, special keys/values).
  - Sequences of operations that stress the abstraction (insert/remove, push/pop, etc.).
- Use implementation-specific subdomains (e.g., “degenerate tree shapes”, “pathological hash patterns”) only when:
  - They are important for performance or correctness of a known implementation, and
  - You accept that these tests may need to change if internals change.

Overall, treat implementation-specific properties as **optional, targeted regression tools**, and build the core property set around **general, abstraction-level requirements** that remain valid across refactorings and alternative implementations.

