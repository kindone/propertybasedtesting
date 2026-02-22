# Malfunction Tolerance Property

**Property**: Component handles erroneous responses from external dependencies gracefully. Component maintains invariants, handles errors correctly, doesn't corrupt state.

**Sources**: Allocator failures, I/O errors, network failures, system call failures.

## Concept

Inject failures from external dependencies (e.g., allocator, I/O) with controlled probability. Verify:
- Component catches and handles failures gracefully
- Component maintains invariants even when failures occur
- Component doesn't corrupt state
- Component can continue operation or clean up properly after failures

**Key idea**: External dependencies can fail; component should handle these failures without corrupting its state or violating invariants.

## Actions

Actions are operations that may trigger failures from external dependencies:

- **Allocation-heavy operations**: Operations that allocate memory (insert, resize, grow)
- **I/O operations**: Read, write, network calls
- **System calls**: File operations, socket operations, etc.
- **Mixed operations**: Sequences combining normal operations and operations that may fail

**Failure injection**: Control probability of failures (e.g., 0% for normal operation, 10-30% for occasional failures, higher for stress tests). Failures should be injected at points where external dependencies are called.

## Checks

After injecting failures and executing actions, verify:

- **Graceful handling**: Component catches exceptions/errors from dependencies
- **Invariant preservation**: Component-specific invariants still hold (e.g., size ≥ 0, no cycles, sorted)
- **State consistency**: Component state is consistent (e.g., successful operations reflected, failed operations don't corrupt state)
- **Partial success handling**: If some operations succeed and others fail, component handles partial state correctly
- **Error state validity**: If component enters error state, it's a valid error state (can be queried, cleaned up, or recovered from)

**State tracking**: Track successful operations (e.g., count successful inserts) and verify component state matches expectations (e.g., size equals successful operations count).

## Subdomains

**No failures**: Normal operation when dependencies work correctly. Baseline behavior.

**Single failures**: Isolated failures (low probability), failures at different points in sequence, failures during critical operations.

**Multiple failures**: High failure rates, cascading failures, multiple failure sources simultaneously, burst failures, failure overload scenarios.
