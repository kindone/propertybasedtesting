# Python-proptest List Shrinking Duplicates Analysis

## Investigation Summary

### Finding 1: Duplicates Only Occur When `minSize > 0`
- ✅ With `minSize=0`: No duplicates found (matches cppproptest behavior)
- ❌ With `minSize > 0`: Duplicates can occur

### Finding 2: Root Cause
When `minSize > 0`, the recursive structure can create the same list through different paths:

**Example: `[4, 1, 1, 4]` with `minSize=2`**

**Level 1 (rear_size=0):**
- `max_front_size = 4 - 0 = 4`
- `min_front_size = 2 - 0 = 2`
- `range_size = 4 - 2 = 2`
- Front sizes: `[2, 3, 4]` (from shrinking 2: `2→1→0`, then +2)
- Creates:
  - `front_size=2`: `[4,1] + [] = [4,1]`
  - `front_size=3`: `[4,1,1] + [] = [4,1,1]`
  - `front_size=4`: `[4,1,1,4] + [] = [4,1,1,4]`

**Level 2 (recursive on `[4,1,1]` with `rear_size=1`):**
- `max_front_size = 3 - 1 = 2`
- `min_front_size = 2 - 1 = 1`
- `range_size = 2 - 1 = 1`
- Front sizes: `[1, 2]` (from shrinking 1: `1→0`, then +1)
- Creates:
  - `front_size=1`: `[4] + [1] = [4,1]` ← **DUPLICATE!**
  - `front_size=2`: `[4,1] + [1] = [4,1,1]`

**The Problem:**
- `[4,1]` is created both:
  1. At Level 1: `front_size=2, rear_size=0` → `[4,1] + [] = [4,1]`
  2. At Level 2: `front_size=1, rear_size=1` → `[4] + [1] = [4,1]`

### Finding 3: Why cppproptest Doesn't Have This Issue
- cppproptest tests for duplicates and expects uniqueness (see `test_shrinker2.cpp`)
- The test `intsN` uses `minSize=0` and expects exactly `2^N` unique values
- When `minSize=0`, our implementation also has no duplicates
- **Hypothesis**: cppproptest's algorithm structure naturally prevents duplicates even when `minSize > 0`, OR cppproptest also has this issue but it's not being tested with `minSize > 0`

### Finding 4: Algorithm Structure
The recursive structure is:
```cpp
rangeShrinkable.flatMap([...](frontSize) -> Shrinkable {
    // Create list with front_size elements + rear elements
}).concat([...](parent) -> stream_t {
    // Recursively shrink with rear_size + 1
    return shrinkFrontAndThenMid(parent, minSize, rearSize + 1).getShrinks();
});
```

This structure can create the same list through different paths when `minSize > 0`.

## Option 2: Structural Fix Discussion

### Challenge
The recursive structure inherently allows the same list to be created through different paths when `minSize > 0`. To fix this structurally, we need to ensure that recursive shrinks are "new" - i.e., they couldn't have been created at the previous level.

### Possible Structural Fixes

#### Option 2A: Filter Based on Rear Part Usage
**Idea**: When `rear_size > 0`, only keep recursive shrinks where the fixed rear part is actually being "used" in a way that makes them different from level 1 lists.

**Challenge**: Hard to determine if a list is "new" without comparing against all previously created lists, which breaks the functional structure.

**Implementation Attempt**: We tried filtering based on whether the recursive shrink has the fixed rear part, but this doesn't prevent duplicates because the same list can have the same rear part.

#### Option 2B: Ensure Recursive Shrinks Are "Strictly Smaller"
**Idea**: When `rear_size > 0`, only create recursive shrinks that are guaranteed to be different from what was created at level 1.

**Challenge**: How do we guarantee this without comparing against all previous lists?

#### Option 2C: Change the Recursive Condition
**Idea**: Modify the condition for when to recursively shrink to avoid creating duplicates.

**Current condition**: `if(parentSize <= minSize || parentSize <= rearSize) return empty;`

**Possible change**: Add additional condition to prevent recursive shrinking when it would create duplicates.

**Challenge**: Hard to determine this without state.

#### Option 2D: Ensure Recursive Shrinks Have Different Length
**Idea**: When `rear_size > 0`, only create recursive shrinks that have a different length from what would be created at level 1.

**Problem**: This doesn't work because:
- Level 1 creates lists of length `[minSize, maxSize]`
- Level 2 creates lists of length `[minSize-1, maxSize-1]` (with fixed rear)
- These ranges can overlap, causing duplicates

#### Option 2E: Track Created Lists (Breaks Functional Structure)
**Idea**: Pass a set of already-created lists through the recursive calls and filter duplicates.

**Problem**: This breaks the functional/pure structure of the algorithm and requires state.

### Recommended Approach

Given the challenges with structural fixes, I recommend:

1. **Accept that duplicates can occur** when `minSize > 0` due to the algorithm structure
2. **Handle duplicates at a higher level** (in the generator) using deduplication
3. **OR investigate cppproptest more deeply** to see if there's a structural difference we're missing

The deduplication approach (Option 1) is the most practical solution that:
- Maintains the functional structure
- Ensures uniqueness as expected by tests
- Matches the behavior expected by cppproptest tests

### Questions for Further Investigation

1. Does cppproptest test for duplicates with `minSize > 0`?
2. Is there a structural difference in how cppproptest's `flatMap`/`concat` work?
3. Does cppproptest's `shrinkIntegral<size_t>` (unsigned) behave differently in a way that prevents duplicates?
4. Is there a subtle difference in the algorithm implementation we're missing?

