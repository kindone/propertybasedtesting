# Crash Tolerance Property

**Property**: Component doesn't crash/abort on any action sequence. After any sequence of actions, component is in valid state (no crashes, no undefined behavior).

## Concept

Generate random sequences of actions (operations) on the component. Execute each sequence and verify:
- No crashes occur during execution
- Component remains in valid state after sequence
- Component can still be used (no corruption)
- Destructor doesn't crash

**Key idea**: Any sequence of valid operations should not crash the component, regardless of order or length.

## Actions

Actions are operations that modify or interact with the component's state:

- **State-modifying operations**: Insert, delete, update, clear, reset, push, pop, etc.
- **State-preserving operations**: Read, query, iterate (may still trigger internal state changes)
- **Edge case operations**: Operations on empty/full/partially-filled component
- **Destructive operations**: Clear, reset, operations that remove all state

**Action sequences**: Generate sequences of varying lengths (short for quick checks, long for stress tests). Weight probabilities to favor common operations but include edge cases.

## Checks

After executing an action sequence, verify:

- **No crashes**: Component didn't abort, segfault, or trigger undefined behavior
- **State validity**: Component is in valid state (can be queried, no corruption)
- **Usability**: Component can still be used (operations can be called, queries return reasonable values)
- **Destructor safety**: Component can be destroyed without crashing

**Validity checks**: Component-specific checks (e.g., for containers: size matches actual elements, iterators work, elements accessible).

## Subdomains

**Ideal conditions**: Normal operations, typical usage patterns, well-formed inputs. Baseline behavior.

**Scenarios requiring handling**: Edge cases, boundary conditions (empty, full, single element), invalid inputs that should be handled gracefully, operations on invalid states.

**Extreme scenarios**: Very long sequences, rapid operations, resource exhaustion, stress tests, fuzzing-style random inputs.
