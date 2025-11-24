# Tuple Shrinking Status

## Current Implementation

### What We Have:

1. **`shrink_pair`** (in `shrinker/pair.py`):
   - Handles tuples of exactly 2 elements
   - Shrinks first element, then second element
   - **NOT recursive** - shrinks are one level deep

2. **`Gen.tuple()`** (in `generator/gen.py`):
   - Creates tuples from multiple generators
   - Has basic shrinking: shrinks each element individually
   - **NOT recursive** - the shrinks it creates are leaf nodes without their own shrinks

### Current `Gen.tuple()` Implementation:

```python
def tuple(*generators):
    def generate(rng: Random) -> Shrinkable[tuple]:
        values = []
        shrinks = []
        for gen in generators:
            shrinkable = gen.generate(rng)
            values.append(shrinkable.value)
            shrinks.append(shrinkable)

        # Create shrinks for the tuple
        tuple_shrinks = []
        for i, shrink in enumerate(shrinks):
            for shr in shrink.shrinks().to_list():
                new_values = values.copy()
                new_values[i] = shr.value
                tuple_shrinks.append(Shrinkable(tuple(new_values)))  # ❌ No recursive shrinks!

        return Shrinkable(tuple(values), lambda: Stream.many(tuple_shrinks))
```

**Problem**: The shrinks created are leaf nodes - they don't have their own shrinks, so shrinking stops after one level.

## cppproptest Implementation

### `shrinkTuple` (in `shrinker/tuple.cpp`):

Uses `shrinkTupleUsingVector` which:
1. Iterates through each position (0 to Size-1)
2. For each position, recursively shrinks that element
3. Creates a full recursive shrink tree

**Example for `(2, "ab", true)`**:
```
(2, "ab", true)
├── (0, "ab", true)          [shrink pos 0: 2 -> 0]
│   ├── (0, "", true)        [shrink pos 1: "ab" -> ""]
│   │   └── (0, "", false)   [shrink pos 2: true -> false]
│   ├── (0, "a", true)       [shrink pos 1: "ab" -> "a"]
│   │   └── (0, "a", false)  [shrink pos 2: true -> false]
│   ├── (0, "b", true)       [shrink pos 1: "ab" -> "b"]
│   │   └── (0, "b", false)  [shrink pos 2: true -> false]
│   └── (0, "ab", false)     [shrink pos 2: true -> false]
├── (1, "ab", true)          [shrink pos 0: 2 -> 1]
│   ├── (1, "", true)        [shrink pos 1: "ab" -> ""]
│   │   └── (1, "", false)    [shrink pos 2: true -> false]
│   ├── (1, "a", true)       [shrink pos 1: "ab" -> "a"]
│   │   └── (1, "a", false)  [shrink pos 2: true -> false]
│   ├── (1, "b", true)       [shrink pos 1: "ab" -> "b"]
│   │   └── (1, "b", false)  [shrink pos 2: true -> false]
│   └── (1, "ab", false)     [shrink pos 2: true -> false]
├── (2, "", true)            [shrink pos 1: "ab" -> ""]
│   └── (2, "", false)       [shrink pos 2: true -> false]
├── (2, "a", true)           [shrink pos 1: "ab" -> "a"]
│   └── (2, "a", false)      [shrink pos 2: true -> false]
├── (2, "b", true)           [shrink pos 1: "ab" -> "b"]
│   └── (2, "b", false)      [shrink pos 2: true -> false]
└── (2, "ab", false)         [shrink pos 2: true -> false]
```

**Key Pattern**: 
- First shrinks position 0 completely (recursively)
- Then for each result, shrinks position 1 completely (recursively)
- Then for each result, shrinks position 2 completely (recursively)
- This creates a full recursive tree

## What's Missing

1. **No general `shrink_tuple` function** - we only have `shrink_pair` for 2 elements
2. **`Gen.tuple()` doesn't create recursive shrinks** - shrinks are one level deep
3. **No `shrinkable_tuple` function** - test expects it but it doesn't exist

## What Needs to Be Done

1. Create a general `shrink_tuple` function (similar to cppproptest's `shrinkTupleUsingVector`)
2. Update `Gen.tuple()` to use this function for proper recursive shrinking
3. The function should:
   - Iterate through each position (0 to N-1)
   - For each position, recursively shrink that element
   - Use `concat` to chain the shrinking strategies
   - Create a full recursive shrink tree

