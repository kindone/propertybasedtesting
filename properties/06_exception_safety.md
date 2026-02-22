# Exception-Safety Property

**Property**: Exceptions don't leave component in invalid state. Component maintains exception-safety guarantees (basic, strong, or nothrow).

**Levels**: Basic guarantee (valid state), strong guarantee (rollback), nothrow guarantee (no exceptions).

## Concept

Inject exceptions at various points during operations. Verify:
- Component catches exceptions and maintains valid state
- Component-specific exception-safety guarantee holds (basic/strong/nothrow)
- No state corruption occurs when exceptions are thrown
- Component can continue operation or clean up properly after exceptions

**Key idea**: Exceptions can be thrown during operations; component should handle them according to its exception-safety guarantee without corrupting state.

## Actions

Focus on operations that may throw exceptions:

- **Copy operations**: Copy constructors, copy assignment (may throw during element copying)
- **Move operations**: Move constructors, move assignment (may throw during resource transfer)
- **Allocation operations**: Operations that allocate (may throw bad_alloc)
- **Operations with preconditions**: Operations that validate inputs (may throw invalid_argument, etc.)

**Exception injection**: Inject exceptions at different phases: before state change, during state change, after partial state change.

## Checks

Verify exception-safety guarantee level:

- **Basic guarantee**: Component is in valid state (can be destroyed, queried)
- **Strong guarantee**: State rolled back to pre-operation state (compare before/after state)
- **Nothrow guarantee**: No exceptions thrown (verify operation completes without throwing)

**State snapshot**: For strong guarantee, capture state before operation, compare after exception to verify rollback.

## Subdomains

**No exceptions**: Operations complete normally. Baseline behavior.

**Exceptions during critical operations**: Exceptions during copy/move, exceptions during allocation, exceptions that should be caught.

**Exception propagation**: Exceptions propagate correctly, exception handling doesn't corrupt state, cleanup on exceptions.
