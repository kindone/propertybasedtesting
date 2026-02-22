# Stateful Testing Framework: Practical Enhancement

## Current Model

The framework currently supports:

1. **Initial state generation**: `Generator<ObjectType>` produces starting states
2. **Action sequence generation**: `Generator<Action>` produces sequences of state-modifying functions
3. **Model factory** (optional): `(ObjectType&) -> ModelType` creates a model from initial state
4. **Actions as functions**: `(ObjectType&) -> void` or `(ObjectType&, ModelType&) -> void`
5. **Assertions**: In actions or post-check callbacks

**Current property shape**: `∀ initial_state ∀ action_sequence P(initial_state, action_sequence)`

This tests: "For all initial states and all action sequences, property P holds."

**Current execution**:
```cpp
for (auto action : actions) {
    action(obj, model);
}
if (postCheck) postCheck(obj, model);
```

---

## Implemented Enhancement: `setOnActionStart` / `setOnActionEnd`

### Problem

For properties like "no intermediate state violates the invariant" (`∀ ∀ ¬∃`), we need to assert after **every action**, not just at the end. Previously, users had to put assertions in every action, which was error-prone.

### Solution (Implemented)

Added `setOnActionStart` and `setOnActionEnd` callbacks:

| Callback | When it runs |
|----------|--------------|
| `setOnActionStart(obj, model)` | Before each action (including before the first) |
| `setOnActionEnd(obj, model)` | After each action |

Use `setOnActionEnd` for invariant checks; use `setOnActionStart` for pre-action checks or debugging.

```cpp
statefulProperty<MyVector>(initialGen, actionGen)
    .setOnActionEnd([](MyVector& vec, EmptyModel&) {
        PROP_ASSERT(vec.size() >= 0);  // Invariant check
    })
    .go();
```

### Benefits

1. **Centralizes invariant checks** — No need to repeat assertions in every action
2. **Reduces errors** — Can't forget to check invariants
3. **Matches quantifier structure** — For `∀ ∀ ¬∃` properties (no intermediate bad state), assert at every step
4. **Flexible** — before/after pair allows both pre-action and post-action checks

---

## Connection to Quantifier View

This enhancement directly addresses the quantifier view insight:

> **"For ∀ ∀ ¬∃ properties (no intermediate bad state), assert at every step."**

The `setOnActionEnd` callback makes it easy to enforce this pattern without requiring users to manually add assertions to every action.

---

## Summary

**Enhancement**: Add `setOnActionStart` and `setOnActionEnd` callbacks.

**Status**: ✅ Implemented
