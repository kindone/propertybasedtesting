# Structured Shrink Tree for Integer List of Size 4

## Example: `[10, 20, 30, 40]`

### Overview

The shrink tree for a list of 4 integers uses two shrinking strategies:

1. **Membership-wise shrinking**: Removes elements from the list
2. **Element-wise shrinking**: Shrinks individual integer elements

---

## 1. Membership-Wise Shrinking Only

This strategy removes elements from the list, trying simpler (shorter) lists first.

```
[10, 20, 30, 40]
│
├── []                                    (empty list - simplest)
│
├── [10, 20, 30]                          (remove last element)
│   ├── []
│   ├── [10, 20]
│   │   ├── []
│   │   └── [10]
│   │       └── []
│   └── [20, 30]
│       ├── []
│       └── [30]
│           └── []
│
├── [20, 30, 40]                          (remove first element)
│   ├── []
│   ├── [30, 40]
│   │   ├── []
│   │   └── [40]
│   │       └── []
│   └── [20, 30]
│       ├── []
│       └── [30]
│           └── []
│
└── [10, 20]                              (remove last 2 elements)
    ├── []
    └── [10]
        └── []
```

**Order of exploration:**
1. `[]` (simplest - empty list)
2. `[10, 20, 30]` (remove last)
3. `[10, 20]` (remove last from previous)
4. `[10]` (remove last from previous)
5. `[20, 30, 40]` (remove first)
6. `[30, 40]` (remove first from previous)
7. `[40]` (remove first from previous)

---

## 2. Element-Wise Shrinking

This strategy shrinks individual elements using integer binary search towards 0.

For each element position, the integer shrinking follows this pattern:

### Element at position 0 (value: 10):
```
10
├── 0   (simplest - shrink to minimum)
├── 5   (midpoint: 0-10)
│   ├── 0
│   ├── 2
│   │   ├── 0
│   │   └── 1
│   └── 7
│       ├── 0
│       ├── 3
│       └── 5
├── 7   (between 5 and 10)
├── 8   (between 7 and 10)
└── 9   (between 8 and 10)
```

### Element at position 1 (value: 20):
```
20
├── 0
├── 10  (midpoint: 0-20)
│   ├── 0
│   ├── 5
│   └── 7
├── 15  (between 10 and 20)
├── 17  (between 15 and 20)
├── 18  (between 17 and 20)
└── 19  (between 18 and 20)
```

### Element at position 2 (value: 30):
```
30
├── 0
├── 15  (midpoint: 0-30)
│   ├── 0
│   ├── 7
│   └── 11
├── 22  (between 15 and 30)
├── 26  (between 22 and 30)
├── 28  (between 26 and 30)
└── 29  (between 28 and 30)
```

### Element at position 3 (value: 40):
```
40
├── 0
├── 20  (midpoint: 0-40)
│   ├── 0
│   ├── 10
│   └── 15
├── 30  (between 20 and 40)
├── 35  (between 30 and 40)
├── 37  (between 35 and 40)
├── 38  (between 37 and 40)
└── 39  (between 38 and 40)
```

---

## 3. Combined Shrink Tree (Membership + Element-wise)

The full shrink tree combines both strategies. Here's the structure showing the first few levels:

```
[10, 20, 30, 40]
│
├── []                                    (membership: empty)
│
├── [10, 20, 30]                          (membership: remove last)
│   ├── []                                (membership: empty)
│   ├── [10, 20]                          (membership: remove last)
│   ├── [20, 30]                          (membership: remove first)
│   └── [10, 20, 0]                       (element-wise: shrink 3rd element)
│   └── [10, 20, 15]                      (element-wise: shrink 3rd element)
│   └── ...                               (more element shrinks)
│
├── [20, 30, 40]                          (membership: remove first)
│   ├── []                                (membership: empty)
│   ├── [30, 40]                          (membership: remove first)
│   ├── [20, 30]                          (membership: remove last)
│   └── [0, 30, 40]                       (element-wise: shrink 1st element)
│   └── [10, 30, 40]                      (element-wise: shrink 1st element)
│   └── ...                               (more element shrinks)
│
├── [0, 20, 30, 40]                       (element-wise: shrink 1st element)
│   ├── [0, 0, 30, 40]                    (element-wise: shrink 2nd element)
│   ├── [0, 10, 30, 40]                   (element-wise: shrink 2nd element)
│   ├── [0, 20, 0, 40]                    (element-wise: shrink 3rd element)
│   └── ...                               (more combinations)
│
├── [5, 20, 30, 40]                       (element-wise: shrink 1st element)
│   └── ...                               (similar structure)
│
├── [10, 0, 30, 40]                       (element-wise: shrink 2nd element)
│   ├── [0, 0, 30, 40]                    (element-wise: shrink 1st too)
│   ├── [10, 0, 0, 40]                    (element-wise: shrink 3rd too)
│   └── ...                               (more combinations)
│
├── [10, 10, 30, 40]                      (element-wise: shrink 2nd element)
│   └── ...                               (similar structure)
│
├── [10, 20, 0, 40]                       (element-wise: shrink 3rd element)
│   └── ...                               (similar structure)
│
└── [10, 20, 30, 0]                       (element-wise: shrink 4th element)
    └── ...                               (similar structure)
```

---

## Key Properties

1. **No Duplicates**: Each list value appears exactly once in the tree, even if it could be reached via multiple paths.

2. **Simplest First**: The shrinker tries the simplest values first:
   - Empty list `[]` is tried first
   - Integer `0` is tried first for each element
   - Then progressively more complex values

3. **Logarithmic Growth**: Integer shrinking uses binary search:
   - First tries `0` (minimum)
   - Then midpoint (e.g., `5` for range 0-10)
   - Then values between midpoint and current (e.g., `7`, `8`, `9`)

4. **Hierarchical Structure**: 
   - Membership-wise shrinking creates shorter lists
   - Element-wise shrinking creates lists with smaller element values
   - Both can be combined recursively

---

## Example Shrink Sequence

If a property fails on `[10, 20, 30, 40]`, the shrinker will try:

1. `[]` - simplest (empty list)
2. `[10, 20, 30]` - remove last element
3. `[10, 20]` - remove last element again
4. `[10]` - remove last element again
5. `[20, 30, 40]` - remove first element
6. `[0, 20, 30, 40]` - shrink first element to 0
7. `[10, 0, 30, 40]` - shrink second element to 0
8. `[10, 20, 0, 40]` - shrink third element to 0
9. `[10, 20, 30, 0]` - shrink fourth element to 0
10. `[5, 20, 30, 40]` - shrink first element to midpoint
11. ... and so on

This continues until a minimal failing case is found.

