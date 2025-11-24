# Shrink Tree Structure from `Gen.list(Gen.int(0, 100), 0, 4)`

## Example Generated List: `[11, 75, 54, 4]`

### Overview

This document shows the actual shrink tree structure extracted from a `Gen.list(Gen.int(0, 100), 0, 4)` generator when it produces a list of size 4.

---

## Shrink Tree Structure

```
[ 11,  75,  54,   4]
│
├── []                                    (membership: empty list - simplest)
│
├── [ 11,  75,  54]                        (membership: remove last element)
│   └── (recursive: can be further shrunk)
│
├── [ 75,  54,   4]                        (membership: remove first element)
│   └── (recursive: can be further shrunk)
│
├── [  0,  75,  54,   4]                  (element-wise: shrink 1st element to 0)
│   └── (recursive: can shrink other elements)
│
├── [  5,  75,  54,   4]                  (element-wise: shrink 1st element to midpoint)
│   └── (recursive: can shrink other elements)
│
├── [  8,  75,  54,   4]                  (element-wise: shrink 1st element)
│
├── [  9,  75,  54,   4]                  (element-wise: shrink 1st element)
│
├── [ 10,  75,  54,   4]                  (element-wise: shrink 1st element)
│
├── [ 11,   0,  54,   4]                  (element-wise: shrink 2nd element to 0)
│   ├── [  0,   0,  54,   4]              (shrink both 1st and 2nd)
│   ├── [ 11,   0,   0,   4]              (shrink 2nd and 3rd)
│   └── ... (more combinations)
│
├── [ 11,  37,  54,   4]                  (element-wise: shrink 2nd element to midpoint)
│   ├── [  0,  37,  54,   4]              (shrink 1st too)
│   └── ... (more combinations)
│
├── [ 11,  56,  54,   4]                  (element-wise: shrink 2nd element)
│
├── [ 11,  65,  54,   4]                  (element-wise: shrink 2nd element)
│
├── [ 11,  70,  54,   4]                  (element-wise: shrink 2nd element)
│
├── [ 11,  72,  54,   4]                  (element-wise: shrink 2nd element)
│
├── [ 11,  73,  54,   4]                  (element-wise: shrink 2nd element)
│
├── [ 11,  74,  54,   4]                  (element-wise: shrink 2nd element)
│
├── [ 11,  75,   0,   4]                  (element-wise: shrink 3rd element to 0)
│   ├── [  0,  75,   0,   4]              (shrink 1st and 3rd)
│   ├── [ 11,   0,   0,   4]              (shrink 2nd and 3rd)
│   └── ... (more combinations)
│
├── [ 11,  75,  27,   4]                  (element-wise: shrink 3rd element to midpoint)
│
├── [ 11,  75,  40,   4]                  (element-wise: shrink 3rd element)
│
├── [ 11,  75,  47,   4]                  (element-wise: shrink 3rd element)
│
├── [ 11,  75,  50,   4]                  (element-wise: shrink 3rd element)
│
├── [ 11,  75,  52,   4]                  (element-wise: shrink 3rd element)
│
├── [ 11,  75,  53,   4]                  (element-wise: shrink 3rd element)
│
├── [ 11,  75,  54,   0]                  (element-wise: shrink 4th element to 0)
│   ├── [  0,  75,  54,   0]              (shrink 1st and 4th)
│   ├── [ 11,   0,  54,   0]              (shrink 2nd and 4th)
│   ├── [ 11,  75,   0,   0]              (shrink 3rd and 4th)
│   └── ... (more combinations)
│
├── [ 11,  75,  54,   2]                  (element-wise: shrink 4th element)
│
└── [ 11,  75,  54,   3]                  (element-wise: shrink 4th element)
    └── ... (more combinations)
```

---

## Shrinking Strategies

### 1. Membership-wise Shrinking

Removes elements from the list:

- **Empty list** `[]` - simplest case, tried first
- **Remove last element**: `[11, 75, 54]` from `[11, 75, 54, 4]`
- **Remove first element**: `[75, 54, 4]` from `[11, 75, 54, 4]`

These shorter lists can then be recursively shrunk further.

### 2. Element-wise Shrinking

Shrinks individual integer elements using binary search towards 0:

For each element position:

1. **Shrink to 0** (simplest)
2. **Shrink to midpoint** (e.g., for 11: midpoint is 5; for 75: midpoint is 37)
3. **Shrink to values between midpoint and current** (e.g., for 11: try 8, 9, 10)

**Example for element at position 0 (value: 11):**
```
11
├── 0   (simplest)
├── 5   (midpoint: 0-11)
├── 8   (between 5 and 11)
├── 9   (between 8 and 11)
└── 10  (between 9 and 11)
```

**Example for element at position 1 (value: 75):**
```
75
├── 0   (simplest)
├── 37  (midpoint: 0-75)
├── 56  (between 37 and 75)
├── 65  (between 56 and 75)
├── 70  (between 65 and 75)
├── 72  (between 70 and 75)
├── 73  (between 72 and 75)
└── 74  (between 73 and 75)
```

---

## Statistics

For the example list `[11, 75, 54, 4]` with depth limit of 5:

- **Total unique values**: 27
- **By length**:
  - Length 0: 1 value (`[]`)
  - Length 3: 2 values (`[11, 75, 54]`, `[75, 54, 4]`)
  - Length 4: 24 values (various combinations with shrunk elements)

---

## Key Properties

1. **Simplest First**: The shrinker tries:
   - Empty list `[]` first
   - Integer `0` for each element first
   - Then progressively more complex values

2. **No Duplicates**: Each list value appears exactly once in the tree

3. **Recursive Structure**: Each shrink candidate can itself be shrunk further, creating a tree structure

4. **Combined Strategies**: The tree combines:
   - Membership-wise shrinking (removing elements)
   - Element-wise shrinking (shrinking integers)
   - Both can be applied recursively

---

## Example Shrink Sequence

If a property fails on `[11, 75, 54, 4]`, the shrinker will try:

1. `[]` - simplest (empty list)
2. `[11, 75, 54]` - remove last element
3. `[75, 54, 4]` - remove first element
4. `[0, 75, 54, 4]` - shrink first element to 0
5. `[11, 0, 54, 4]` - shrink second element to 0
6. `[11, 75, 0, 4]` - shrink third element to 0
7. `[11, 75, 54, 0]` - shrink fourth element to 0
8. `[5, 75, 54, 4]` - shrink first element to midpoint
9. `[11, 37, 54, 4]` - shrink second element to midpoint
10. ... and so on

This continues recursively until a minimal failing case is found.

