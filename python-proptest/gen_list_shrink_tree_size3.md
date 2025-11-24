# Complete Shrink Tree Structure from `Gen.list(Gen.int(0, 100), 0, 3)`

## Example Generated List: `[4, 3, 11]`

### Overview

This document shows the **complete** shrink tree structure extracted from a `Gen.list(Gen.int(0, 100), 0, 3)` generator when it produces a list of size 3.

---

## Complete Shrink Tree Structure

```
[  4,   3,  11]
│
├── []                                    (membership: empty list - simplest)
│
├── [  4,   3]                             (membership: remove last element)
│   └── (can be further shrunk recursively)
│
├── [  3,  11]                             (membership: remove first element)
│   └── (can be further shrunk recursively)
│
├── [  0,   3,  11]                        (element-wise: shrink 1st element to 0)
│   └── (can shrink other elements recursively)
│
├── [  2,   3,  11]                        (element-wise: shrink 1st element to midpoint)
│   └── (can shrink other elements recursively)
│
├── [  3,   3,  11]                        (element-wise: shrink 1st element)
│   └── (can shrink other elements recursively)
│
├── [  4,   0,  11]                        (element-wise: shrink 2nd element to 0)
│   └── (can shrink other elements recursively)
│
├── [  4,   1,  11]                        (element-wise: shrink 2nd element)
│   └── (can shrink other elements recursively)
│
├── [  4,   2,  11]                        (element-wise: shrink 2nd element)
│   └── (can shrink other elements recursively)
│
├── [  4,   3,   0]                        (element-wise: shrink 3rd element to 0)
│   └── (can shrink other elements recursively)
│
├── [  4,   3,   5]                        (element-wise: shrink 3rd element to midpoint)
│   └── (can shrink other elements recursively)
│
├── [  4,   3,   8]                        (element-wise: shrink 3rd element)
│   └── (can shrink other elements recursively)
│
├── [  4,   3,   9]                        (element-wise: shrink 3rd element)
│   └── (can shrink other elements recursively)
│
└── [  4,   3,  10]                        (element-wise: shrink 3rd element)
    └── (can shrink other elements recursively)
```

---

## All Unique Values in Shrink Tree

Sorted by length, then by values:

### Length 0 (1 value):
- `[]`

### Length 2 (2 values):
- `[3, 11]` (remove first element)
- `[4, 3]` (remove last element)

### Length 3 (12 values):
- `[0, 3, 11]` (shrink 1st element to 0)
- `[2, 3, 11]` (shrink 1st element to midpoint)
- `[3, 3, 11]` (shrink 1st element)
- `[4, 0, 11]` (shrink 2nd element to 0)
- `[4, 1, 11]` (shrink 2nd element)
- `[4, 2, 11]` (shrink 2nd element)
- `[4, 3, 0]` (shrink 3rd element to 0)
- `[4, 3, 5]` (shrink 3rd element to midpoint)
- `[4, 3, 8]` (shrink 3rd element)
- `[4, 3, 9]` (shrink 3rd element)
- `[4, 3, 10]` (shrink 3rd element)
- `[4, 3, 11]` (original)

---

## Shrinking Strategies Explained

### 1. Membership-wise Shrinking

Removes elements from the list:

1. **Empty list** `[]` - simplest case, tried first
2. **Remove last element**: `[4, 3]` from `[4, 3, 11]`
3. **Remove first element**: `[3, 11]` from `[4, 3, 11]`

These shorter lists can then be recursively shrunk further (not shown in first level, but they have their own shrink trees).

### 2. Element-wise Shrinking

Shrinks individual integer elements using binary search towards 0.

#### Element at position 0 (value: 4):
```
4
├── 0   (simplest - shrink to minimum)
├── 2   (midpoint: 0-4)
└── 3   (between 2 and 4)
```

**Shrinks tried:**
- `[0, 3, 11]` - shrink to 0
- `[2, 3, 11]` - shrink to midpoint
- `[3, 3, 11]` - shrink to value between midpoint and current

#### Element at position 1 (value: 3):
```
3
├── 0   (simplest - shrink to minimum)
├── 1   (between 0 and 3)
└── 2   (between 1 and 3)
```

**Shrinks tried:**
- `[4, 0, 11]` - shrink to 0
- `[4, 1, 11]` - shrink to value between 0 and 3
- `[4, 2, 11]` - shrink to value between 1 and 3

#### Element at position 2 (value: 11):
```
11
├── 0   (simplest - shrink to minimum)
├── 5   (midpoint: 0-11)
├── 8   (between 5 and 11)
├── 9   (between 8 and 11)
└── 10  (between 9 and 11)
```

**Shrinks tried:**
- `[4, 3, 0]` - shrink to 0
- `[4, 3, 5]` - shrink to midpoint
- `[4, 3, 8]` - shrink to value between midpoint and current
- `[4, 3, 9]` - shrink to value between 8 and 11
- `[4, 3, 10]` - shrink to value between 9 and 11

---

## Statistics

For the example list `[4, 3, 11]`:

- **Total unique values**: 15
- **By length**:
  - Length 0: 1 value (`[]`)
  - Length 2: 2 values (`[4, 3]`, `[3, 11]`)
  - Length 3: 12 values (various combinations with shrunk elements)

- **By strategy**:
  - Empty lists: 1
  - Membership shrinks (length < 3): 2
  - Element-wise shrinks (length = 3): 12

---

## Key Properties

1. **Simplest First**: The shrinker tries:
   - Empty list `[]` first
   - Integer `0` for each element first
   - Then progressively more complex values

2. **No Duplicates**: Each list value appears exactly once in the tree

3. **Recursive Structure**: Each shrink candidate can itself be shrunk further:
   - `[4, 3]` can be shrunk to `[]`, `[4]`, `[3]`, or with element shrinks
   - `[3, 11]` can be shrunk to `[]`, `[3]`, `[11]`, or with element shrinks
   - `[0, 3, 11]` can have its 2nd and 3rd elements shrunk further

4. **Combined Strategies**: The tree combines:
   - Membership-wise shrinking (removing elements)
   - Element-wise shrinking (shrinking integers)
   - Both can be applied recursively

---

## Example Shrink Sequence

If a property fails on `[4, 3, 11]`, the shrinker will try:

1. `[]` - simplest (empty list)
2. `[4, 3]` - remove last element
3. `[3, 11]` - remove first element
4. `[0, 3, 11]` - shrink first element to 0
5. `[4, 0, 11]` - shrink second element to 0
6. `[4, 3, 0]` - shrink third element to 0
7. `[2, 3, 11]` - shrink first element to midpoint
8. `[4, 1, 11]` - shrink second element
9. `[4, 3, 5]` - shrink third element to midpoint
10. ... and so on

This continues recursively until a minimal failing case is found.

---

## Comparison: Size 3 vs Size 4

**Size 3** (`[4, 3, 11]`):
- Total unique values: 15
- More manageable for analysis
- Clear structure visible

**Size 4** (`[11, 75, 54, 4]`):
- Total unique values: 27+ (with depth limit)
- More complex due to additional element
- Still follows same patterns but with more combinations

The structure scales predictably: each additional element adds more shrink candidates, but the fundamental strategies (membership-wise and element-wise) remain the same.

